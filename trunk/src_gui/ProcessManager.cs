/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    GeoGen is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GeoGen.  If not, see <http://www.gnu.org/licenses/>.

*/

using System;
using System.Collections;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Diagnostics;
using System.Threading;
using System.Linq;

using GeoGen.Net;

namespace GeoGen.Studio
{
    public partial class Main
    {
        private Generator ggen;

        private enum Mode
        {
            Standard, // a script is being executed
            SyntaxCheck // syntax check is in progress
        };

        private class BenchmarkStatus
        {
            public Int16 phase = 0;
            public Int64 thisBenchTime;
            public Double sumScores = 0;
        };

        private Mode mode;

        private System.Threading.Thread GGenThread = null;

        public string lastCheckContent;
        private bool isCheckScheduled;

        private BenchmarkStatus benchmarkStatus;
        private BenchmarkForm benchmarkForm;

        public Hashtable maps;

        public Int64 startTime;

        private uint[] oldValues;

        public void InitializeGGen()
        {
            this.mode = Mode.Standard;

            this.maps = new Hashtable();

            this.ggen = new Generator();

            this.ggen.MapReturned += ReturnHandler;
            this.ggen.ProgressChanged += ProgressHandler;
            this.ggen.MessageThrown += PrintHandler;

            this.ggen.MaximumMapSize = (int)this.config.maxMapSize;
            this.ggen.MaximumMapCount = (int)this.config.maxMapCount;

            this.oldValues = null;
        }

        public bool IsGGenRunning(){
            //return mode != Mode.Idle;   

            return this.GGenThread != null && this.GGenThread.IsAlive;
        }

        public void ScheduleSyntaxCheck(){
            if(this.IsGGenRunning()) this.isCheckScheduled = true;
            else this.ExecuteScript(this.GetScript(), true, null);
        }

        public bool InBenchmarkInProgress(){
            return benchmarkStatus != null;
        }

        public void ExecuteScheduledCheck()
        {
            if (isCheckScheduled)
            {
                this.isCheckScheduled = false;
                this.ExecuteScript(this.GetScript(), true, null);
            }
        }

        public void CancelBenchmark()
        {
            this.Terminate();

            this.benchmarkStatus = null;
        }

        public void ReturnHandler(object sender, MapReturnedEventArgs e){
            Config config = Main.Get().GetConfig();

            HeightData data = Main.GetResizedHeightData(e.HeightMap, Math.Min(e.HeightMap.Width, (int)config.mapDetailLevel), Math.Min(e.HeightMap.Height, (int)config.mapDetailLevel));

            e.HeightMap.Dispose();

            Main.StretchHeightValues(ref data);

            if (this.maps.Contains(e.Label))
            {
                this.WriteToConsole("Warning: Map with label \"" + e.Label + "\" was returned more than once!");
                this.maps[e.Label] = data;
            }
            else
            {
                this.maps.Add(e.Label, data);
            }
        }

        public void ProgressHandler(object sender, ProgressEventArgs e)
        {
            this.Invoke(new MethodInvoker(delegate()
            {
                this.progress.Maximum = e.MaximumProgress;
                this.progress.Value = e.CurrentProgress;
            }));
        }

        public void PrintHandler(object sender, MessageEventArgs e)
        {
            this.Invoke(new MethodInvoker(delegate
            {
                string level = "";

                switch (e.Level)
                {
                    case MessageLevel.Error: level = "Error"; break;
                    case MessageLevel.Warning: level = "Warning"; break;
                    case MessageLevel.Message: level = "Message"; break;
                    case MessageLevel.Notice: level = "Notice"; break;
                }

                if (this.mode == Mode.Standard) this.WriteToConsole(level + ": " + e.Message);
                else this.lastCheckContent += level + ": " + e.Message + Environment.NewLine;
            }));
        }

        public void ClearData()
        {
            this.currentImportedFile = null;

            // free the pointers (so garbage collector can do its job)
            this.output.Image = null;
            this.currentImage = null;
            this.currentImageWithOverlay = null;

            // empty the output list
            this.outputs.Items.Clear();

            if (this.output.Image != null)
            {               
                // collect the garbage (there are now possibly hundreds of megabytes of garbage laying around by now)
                System.GC.Collect();

                // reset the image size (so it doesn't show error image)
                this.output.Width = 0;
                this.output.Height = 0;
            }

            foreach (DictionaryEntry data in this.maps)
            {
                ((HeightData) data.Value).Dispose();
            }

            this.maps.Clear();

            this.ClearData3D();
        }

        public void MapGenerationFailed(string message)
        {
            this.Invoke(new MethodInvoker(delegate()
            {
                if (this.mode == Mode.SyntaxCheck)
                {
                    this.SyntaxCheckFailed();
                }
                else
                {
                    this.WriteToConsole(message + Environment.NewLine);

                    if (this.benchmarkStatus != null) this.BenchmarkFailed();

                    this.ButtonsNoRunMode();

                    this.ExecuteScheduledCheck();

                    this.RemoveStatus("Generating");
                }
            }));
        }

        public void SyntaxCheckFailed()
        {
            this.RemoveStatus("Checking syntax");
            this.SetErrorStatus(true);
        }

        public void ExecuteScript(string script, bool syntaxCheckOnly, uint[] parameters)
        {
            // kill current syntax check in progress (if any is running)
            if (this.IsGGenRunning() && mode == Mode.SyntaxCheck && syntaxCheckOnly == false)
            {
                // wait for the syntax check to finish (it usually takes just milliseconds)
                this.GGenThread.Join();

                // do the check one the requested script is finished
                this.ScheduleSyntaxCheck();
            }
            // syntax checks are not allowed while a script is being executed
            else if (this.IsGGenRunning() && syntaxCheckOnly == true)
            {
                // do the check one the requested script is finished
                if(!this.isCheckScheduled) this.ScheduleSyntaxCheck();

                return;
            }
            // having two generators running at once shouldn't happen
            else if (this.IsGGenRunning() && mode == Mode.Standard)
            {
                throw new Exception("Two GeoGens can't possibly be running at one time");
            }

            if (!syntaxCheckOnly)
            {
                this.mode = Mode.Standard;
                this.ButtonsRunMode();
                this.AddStatus("Executing");

                this.ClearData();

                this.WriteToConsole("Starting at " + DateTime.Now.ToString("HH:mm:ss") + "...");
            }
            else{
                this.mode = Mode.SyntaxCheck;
                
                this.lastCheckContent = "";
                this.AddStatus("Checking syntax");
            }

            System.Threading.ThreadStart starter = new System.Threading.ThreadStart(delegate
            {
                try
                {
                    try
                    {
                        ggen.SetScript(script);
                    }
                    catch (SyntaxErrorException)
                    {
                        this.MapGenerationFailed("Compilation failed!");

                        return;
                    }

                    try
                    {
                        ggen.LoadArgs();
                    }
                    catch (ArgsUnreadableException)
                    {
                        this.MapGenerationFailed("Map header is unreadable!");

                        return;
                    }

                    // do not generate the map during syntax check runs
                    if (!syntaxCheckOnly)
                    {
                        // no list of arguments was passed to the function -> use params from the param table
                        if (parameters == null)
                        {
                            var zipped = this.parameters.Item.Cast<PropertyGridEx.CustomProperty>().Zip(ggen.Args, (Property, Arg) => new { Property, Arg });

                            foreach (var item in zipped)
                            {
                                // fill in only matching arguments
                                if (item.Property.Type.Name == "Boolean" && item.Arg.Type == ScriptArgType.Bool)
                                {
                                    item.Arg.Value = (uint)item.Property.Value;
                                }

                                else if (item.Property.Type.Name == "UInt32" && item.Arg.Type == ScriptArgType.Int)
                                {
                                    item.Arg.Value = (uint)item.Property.Value;
                                }
                                else if (item.Property.Type.Name == "Int32" && item.Arg.Type == ScriptArgType.Int)
                                {
                                    item.Arg.Value = (uint)((int)item.Property.Value);
                                }
                                else if (item.Property.Type.Name == "String" && item.Arg.Type == ScriptArgType.Enum)
                                {
                                    item.Arg.Value = (uint)item.Property.Choices.IndexOf(item.Property.Value);
                                }
                            }

                            /*int i = 0;
                            foreach (PropertyGridEx.CustomProperty property in this.parameters.Item)
                            {
                                
                                
                                Generator.ScriptArg arg = ggen.Args[i];

                                // we ran out of parameters...
                                if (i == ggen.Args.Count())
                                {
                                    break;
                                }

                                // fill in only matching arguments
                                if (property.Type.Name == "Boolean" && arg.Type == ScriptArgType.Bool)
                                {
                                    arg.Value = (uint)property.Value;
                                }

                                else if (property.Type.Name == "UInt32" && arg.Type == ScriptArgType.Int)
                                {
                                    arg.Value = (uint)property.Value;
                                }
                                else if (property.Type.Name == "Int32" && arg.Type == ScriptArgType.Int)
                                {
                                    arg.Value = (uint)((int)property.Value);
                                }
                                else if (property.Type.Name == "String" && arg.Type == ScriptArgType.Enum)
                                {
                                    arg.Value = (uint)property.Choices.IndexOf(property.Value);
                                }

                                i++;
                            }*/
                        }
                        // the argument list was passed to the function
                        else
                        {
                            var zipped = parameters.Zip(ggen.Args, (Param, Arg) => new { Param, Arg });

                            foreach (var item in zipped)
                            {
                                item.Arg.Value = item.Param;
                            }

                            /*int i = 0;
                            foreach (uint currentParam in parameters)
                            {
                                Generator.ScriptArg arg = ggen.Args[i];

                                // we ran out of parameters...
                                if (i == ggen.Args.Length)
                                {
                                    break;
                                }

                                arg.Value = currentParam;

                                i++;
                            }*/
                        }

                        this.startTime = System.DateTime.Now.Ticks / 10000;

                        HeightData result;
                        try
                        {
                            if (this.config.seed == 0)
                            {
                                ggen.Seed = (uint)DateTime.Now.Ticks;
                            }
                            else
                            {
                                ggen.Seed = this.config.seed;
                            }

                            result = ggen.Generate();
                        }
                        catch (GenerationFailedException)
                        {
                            this.MapGenerationFailed("Map generation failed!");

                            return;
                        }
                        catch (ExceptionInCallbackException e)
                        {
                            // the thread was aborted
                            if (e.InnerException is ThreadAbortException)
                            {
                                this.ggen.Reset();
                                this.BeginInvoke(new MethodInvoker(delegate()
                                {
                                    this.MapGenerationFailed("Aborted!");
                                }));
                                return;
                            }

                            // else unknown exception, we want to debug this case
                            throw;
                        }

                        // map was generated successfully
                        HeightData result2 = Main.GetResizedHeightData(result, Math.Min(result.Width, (int)config.mapDetailLevel), Math.Min(result.Height, (int)config.mapDetailLevel));

                        result.Dispose();

                        Main.StretchHeightValues(ref result2);

                        maps.Add("[Main]", result2);

                        this.RemoveStatus("Executing");

                        this.Invoke(new MethodInvoker(delegate()
                        {
                            // was this part of a benchmark?
                            if (benchmarkStatus != null)
                            {
                                this.Benchmark();

                                return;
                            }

                            this.ReloadMaps(null);

                            this.WriteToConsole("Finished after " + Math.Round((System.DateTime.Now.Ticks / 10000 - this.startTime) / 1000d, 3) + " seconds!" + Environment.NewLine);

                            this.ButtonsNoRunMode();                            
                        }));
                    }
                    else
                    {
                        this.Invoke(new MethodInvoker(delegate()
                        {
                            this.RebuildArgsTable();
                            this.SetErrorStatus(false);
                        }));
                    }
                }
                catch (InternalErrorException e)
                {
                    this.WriteToConsole("Error: " + e.InnerException.Message);
                    this.MapGenerationFailed("GeoGen has unexpectedly crashed!");
                }
                finally
                {
                    this.BeginInvoke(new MethodInvoker(delegate()
                    {
                        this.RemoveStatus("Executing");
                        this.RemoveStatus("Checking syntax");
                        this.ExecuteScheduledCheck();
                    }));
                }
            });

            GGenThread = new System.Threading.Thread(starter);

            GGenThread.Start();
        }

        public uint[] GetArgValues()
        {
            uint[] arr = new uint[this.parameters.Item.Count];

            for (int i = 0; i < (int)this.parameters.Item.Count; i++)
            {
                PropertyGridEx.CustomProperty property = this.parameters.Item[i];

                // bool type field
                if (property.Type.Name == "Boolean")
                {
                    arr[i] = ((Boolean)property.Value) == true ? 1u : 0u;
                }
                // int type field
                else if (property.Type.Name == "Int32")
                {
                    arr[i] = (uint)((int)property.Value);
                }
                else if (property.Type.Name == "UInt32")
                {
                    arr[i] = (uint)property.Value;
                }
                // enum type field
                else if (property.Type.Name == "String")
                {
                    arr[i] = (uint) property.Choices.LastIndexOf((String)(property.Value));
                }
            }

            return arr;
        }

        public void RebuildArgsTable()
        {
            if (this.oldValues == null) this.oldValues = this.GetArgValues();

            this.parameters.Item.Clear();

            int i = 0;
            foreach (ScriptArg arg in ggen.Args)
            {
                PropertyGridEx.CustomProperty property = null;

                if (arg.Type == ScriptArgType.Bool)
                {
                    property = new PropertyGridEx.CustomProperty(arg.Label, arg.Default == 1 ? true : false, false, "Script parameters:", arg.Description, true);

                    if (oldValues.Length > i) property.Value = oldValues[i] == 1 ? true : false;
                }
                else if (arg.Type == ScriptArgType.Int)
                {
                    property = new PropertyGridEx.CustomProperty(arg.Label, (UInt32) arg.Default, false, "Script parameters:", arg.Description, true);

                    if (oldValues.Length > i) property.Value = (uint) (Math.Max(Math.Min(oldValues[i], arg.Maximum), arg.Minimum));
                }
                else if (arg.Type == ScriptArgType.Enum)
                {
                    property = new PropertyGridEx.CustomProperty(arg.Label, arg.Options.ElementAt((int) arg.Default), false, "Script parameters:", arg.Description, true);
                    property.Choices = new PropertyGridEx.CustomChoices(arg.Options.ToArray(), false);

                    if (oldValues.Length > i) property.Value = arg.Options.Count() > oldValues[i] ? arg.Options.ElementAt((int)oldValues[i]) : arg.Options.ElementAt((int)arg.Default);
                }

                if (property != null)
                {
                    this.parameters.Item.Add(property);
                }

                i++;
            }

            this.oldValues = null;

            this.parameters.Refresh();
        }

        public void Terminate(){
            if (this.GGenThread != null && this.GGenThread.IsAlive)
            {
                this.GGenThread.Abort();
            }
        }

        public void ShowLastErrorInfo()
        {
            this.WriteToConsole("Last error info:");
            this.WriteToConsole(this.lastCheckContent);
        }

        public string GetParamString()
        {
            string str = "";

            foreach (PropertyGridEx.CustomProperty param in this.parameters.Item)
            {
                // default value is being used
                if (param.Value == param.DefaultValue)
                {
                    str += "d ";
                }
                // integer type
                else if (param.Type.Name == "UInt32" || param.Type.Name == "Int32")
                {
                    str += (Int32)param.Value + " ";
                }
                // boolean type
                else if (param.Type.Name == "Boolean")
                {
                    str += ((Boolean)param.Value) ? "1 " : "0 ";
                }
                // enum type
                else
                {
                    str += param.Choices.LastIndexOf((String)(param.Value)) + " ";
                }
            }

            return str;
        }

        public void StartBenchmark()
        {
            this.benchmarkForm = new BenchmarkForm();
            benchmarkForm.ShowDialog();
        }

        public void BenchmarkFailed()
        {
            this.Invoke(new MethodInvoker(delegate()
            {
                this.WriteToConsole("Benchmark failed!" + Environment.NewLine);

                this.benchmarkStatus = null;

                if (this.benchmarkForm != null)
                {
                    this.benchmarkForm.result.Text = "Benchmark failed!";
                    this.benchmarkForm.result.ForeColor = System.Drawing.Color.Red;
                    this.benchmarkForm.result.Visible = true;
                    this.benchmarkForm.button.Text = "Close";
                    this.benchmarkForm.progressBar.Visible = false;
                }

                this.ButtonsNoRunMode();
            }));
        }

        public void Benchmark()
        {
            string benchScript = System.IO.File.ReadAllText(Program.BasePath + "examples/basic.nut");

            uint[][] parameters = new uint[][] {
                new uint[]{1024, 1024},
                new uint[]{2048, 2048},
                new uint[]{4096, 4096}
            };

            int totalPhases = 2 * parameters.Length;

            this.GGenThread = null;

            // the benchmark is starting right now
            if (this.benchmarkStatus == null)
            {
                this.benchmarkStatus = new BenchmarkStatus();

                this.WriteToConsole(Environment.NewLine + "Starting benchmark..." + Environment.NewLine);

                this.ExecuteScript(benchScript, false, parameters[0]);

                this.WriteToConsole("Starting reference script...");

                this.benchmarkForm.progressBar.Maximum = totalPhases;

                this.startTime = System.DateTime.Now.Ticks / 10000;
            }
            // the benchmark is already running
            else
            {
                Int64 time = System.DateTime.Now.Ticks / 10000 - this.startTime;

                // the benchmark script was executed
                if (this.benchmarkStatus.phase % 2 == 0)
                {
                    this.benchmarkStatus.thisBenchTime = time;

                    if (++this.benchmarkStatus.phase < totalPhases)
                    {
                        this.WriteToConsole("Starting benchmarked script...");
                        this.ExecuteScript(this.GetScript(), false, parameters[this.benchmarkStatus.phase / 2]);
                    }
                }
                // the user script was executed
                else
                {
                    this.benchmarkStatus.sumScores += (Double) time / (Double) this.benchmarkStatus.thisBenchTime;

                    if (++this.benchmarkStatus.phase < totalPhases)
                    {
                        this.WriteToConsole("Starting reference script...");
                        this.ExecuteScript(benchScript, false, parameters[this.benchmarkStatus.phase / 2]);
                    }
                }

                this.benchmarkForm.progressBar.Value = this.benchmarkStatus.phase;

                //this.benchmarkStatus.phaseStartTime = System.DateTime.Now.Ticks / 10000;

                // benchmark is finished
                if (this.benchmarkStatus.phase == totalPhases)
                {
                    Double score = Math.Round(this.benchmarkStatus.sumScores / parameters.Length, 2);

                    this.WriteToConsole("Benchmark finished with score " + score.ToString() + "x");

                    this.benchmarkStatus = null;

                    this.benchmarkForm.result.Text = "Benchmark Score: " + score.ToString() + "x";
                    this.benchmarkForm.result.Visible = true;
                    this.benchmarkForm.button.Text = "Close";
                    this.benchmarkForm.progressBar.Visible = false;

                    this.ButtonsNoRunMode();
                }
            }
        }
    }
}
