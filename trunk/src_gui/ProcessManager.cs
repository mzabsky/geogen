using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Diagnostics;

namespace GeoGen_Studio
{
    public class ProcessManager
    {
        private enum Mode
        {
            Idle, // geogen is not running
            Standard, // a script is being executed
            LowPriority // syntax check is in progress
        };

        private class BenchmarkStatus
        {
            public Int64 phaseStartTime;
            public Int16 phase = 0;
            public Int64 thisBenchTime;
            public Double sumScores = 0;
        };

        private Mode mode;
        public Process process;
        
        public string lastCheckContent;
        private bool isCheckScheduled;

        private BenchmarkStatus benchmarkStatus;
        private BenchmarkForm benchmarkForm;

        public ProcessManager()
        {
            mode = Mode.Idle;
        }

        public bool IsRunning(){
            return mode != Mode.Idle;   
        }

        public void ScheduleCheck(){
            if(this.IsRunning()) this.isCheckScheduled = true;
            else this.ExecuteScript(Main.Get().GetScript(), true, "");
        }

        public bool InBenchmarkInProgress(){
            return benchmarkStatus != null;
        }

        public void ExecuteScheduledCheck()
        {
            if (isCheckScheduled)
            {
                this.isCheckScheduled = false;
                this.ExecuteScript(Main.Get().GetScript(), true, "");
            }
        }

        public void CancelBenchmark()
        {
            this.Terminate();

            this.benchmarkStatus = null;
        }

        private void Prepare()
        {
            Main main = Main.Get();
            Config config = main.GetConfig();

            try
            {
                main.outputManager.ClearData();
            }
            catch (Exception)
            {
                main.WriteToConsole("Directory preparation failed. Make sure the necessary files and folders are readable.");
            }
        }

        public void ExecuteScript(string script, bool verificationOnly, string parameters)
        {
            Main main = Main.Get();
            Config config = main.GetConfig();

            // kill current syntax check in progress (if any is running)
            if (mode == Mode.LowPriority)
            {
                process.Kill();

                // do the check one the requested script is finished
                this.ScheduleCheck();
            }
            // syntax checks are not allowed while a script is being executed
            else if (mode == Mode.Standard && verificationOnly == true)
            {
                // do the check one the requested script is finished
                this.ScheduleCheck();

                return;
            }
            // having two generators running at once shouldn't happen
            else if (mode == Mode.Standard)
            {
                process.Kill();
                System.Windows.Forms.MessageBox.Show("Another child GeoGen process is still running, terminating.");
            }

            if (!verificationOnly)
            {
                main.ButtonsRunMode();
            }
            else{
                this.lastCheckContent = "";
            }

            string inPath;
            string outPath;

            // calculate absolute paths (geogen is not interested in paths relative to this executable nor path with a thousand /../ in th middle)
            System.IO.DirectoryInfo inPathInfo = new System.IO.DirectoryInfo(config.GeoGenWorkingDirectory + "/" + config.ScriptTempFile);
            inPath = inPathInfo.FullName;

            System.IO.DirectoryInfo outPathInfo = new System.IO.DirectoryInfo(config.GeoGenWorkingDirectory + "/" + config.MainMapOutputFile);
            outPath = outPathInfo.FullName;

            if(!verificationOnly) this.Prepare();

            // write the temporary script file
            System.IO.File.WriteAllText(config.GeoGenWorkingDirectory + "/" + config.ScriptTempFile, script);

            string params_str;

            if (parameters != "")
            {
                params_str = parameters;
            }
            else if (!verificationOnly)
            {
                params_str = GetParamString();
            }
            else
            {
                params_str = " -p";
            }

            this.process = new System.Diagnostics.Process();
            this.process.StartInfo.FileName = config.GeoGenPath;
            this.process.StartInfo.RedirectStandardOutput = true;
            this.process.StartInfo.RedirectStandardError = true;
            this.process.StartInfo.UseShellExecute = false;
            this.process.StartInfo.CreateNoWindow = true;
            this.process.StartInfo.WorkingDirectory = config.GeoGenWorkingDirectory + "/";
            this.process.StartInfo.Arguments = " -i \"" + inPath + "\" -o \"" + outPath + "\" " + params_str;
            this.process.EnableRaisingEvents = true;

            if (!verificationOnly)
            {
                // call the processFinished method once the generator finishes
                this.process.Exited += delegate(object sender, EventArgs e)
                {
                    System.Threading.Thread.Sleep(50);
                    Main.Get().Invoke(new MethodInvoker(delegate()
                    {
                        Main.Get().GetProcessManager().ProcessFinished();
                    }));
                };

                // write every line of output to console
                this.process.OutputDataReceived += delegate(Object sender, System.Diagnostics.DataReceivedEventArgs data)
                {
                    Main.Get().Invoke(new MethodInvoker(delegate()
                    {
                        Main.Get().WriteToConsole(data.Data);
                    }));
                };

                main.WriteToConsole("Launching 'geogen " + process.StartInfo.Arguments + "'...");

                this.mode = Mode.Standard;
            }
            else
            {
                // write every line of output to console
                this.process.OutputDataReceived += delegate(Object sender, System.Diagnostics.DataReceivedEventArgs data)
                {
                    Main.Get().Invoke(new MethodInvoker(delegate()
                    {
                        Main.Get().GetProcessManager().lastCheckContent += data.Data + Environment.NewLine;
                    }));
                };
                
                // call the processFinished method once the generator finishes
                this.process.Exited += delegate(object sender, EventArgs e)
                {
                    System.Threading.Thread.Sleep(50);
                    Main.Get().Invoke(new MethodInvoker(delegate()
                    {
                        Main.Get().GetProcessManager().SyntaxCheckFinished();
                    }));
                };

                this.mode = Mode.LowPriority;
            }

            this.process.Start();

            process.BeginOutputReadLine();
        }

        public void ProcessFinished(){
            Main main = Main.Get();

            // was this part of a benchmark?
            if (benchmarkStatus != null)
            {
                this.Benchmark();

                return;
            }

            main.ButtonsNoRunMode();

            if (process.ExitCode == 0)
            {
                // let the output manager load the outputs
                main.GetOutputManager().CaptureOutputs();
            }
            // exit code != 0 means error of some sort
            else
            {
                // let the application sleep for a moment so all the geogen runtime errors can be printed correctly first
                main.WriteToConsole("The generator was terminated prematurely!");
            }

            this.mode = Mode.Idle;

            this.ExecuteScheduledCheck();
        }

        public void SyntaxCheckFinished()
        {
            Main main = Main.Get();
            
            main.SetErrorStatus(process.ExitCode == 0 ? false : true);

            // if everything was okay, rebuild the parameter propertygrid
            if (process.ExitCode == 0)
            {
                // parameter string representing the values before this propertygrid rebuild
                string oldStr = "";

                if (main.parameters.Item.Count > 0)
                {
                    oldStr = this.GetParamString();
                }

                string[] values = oldStr.Split(' ');

                main.parameters.Item.Clear();

                string[] lines = System.Text.RegularExpressions.Regex.Split(this.lastCheckContent, "\r\n");

                // skip the first four lines (just initialization crap)
                for (int i = 4; i < lines.Length; i++)
                {
                    if (lines[i] == "END") break;

                    string[] parts = lines[i].Split(';');

                    PropertyGridEx.CustomProperty property;

                    // BOOLEAN TYPE
                    if (parts[2] == "B")
                    {
                        property = new PropertyGridEx.CustomProperty(parts[0], parts[3] == "1" ? true : false, false, "Script parameters:", parts[1], true);

                        // try to preserve the original value
                        if (values.Length - 1 > i - 4 && values[i - 4] != "d")
                        {
                            property.Value = values[i - 4] == "0" ? false : true;
                        }
                    }
                    // INTEGER TYPE
                    else if (parts[2] == "I")
                    {
                        property = new PropertyGridEx.CustomProperty(parts[0], Int32.Parse(parts[3]), false, "Script parameters:", parts[1], true);

                        // try to preserve the original value
                        if (values.Length - 1 > i - 4 && values[i - 4] != "d")
                        {
                            property.Value = Int32.Parse(values[i - 4]);
                        }
                    }
                    // ENUM TYPE
                    else
                    {
                        property = new PropertyGridEx.CustomProperty(parts[0], parts[8].Split(',')[Int32.Parse(parts[3])], false, "Script parameters:", parts[1], true);
                        property.Choices = new PropertyGridEx.CustomChoices(parts[8].Split(','), false);

                        // try to preserve the original value
                        if (values.Length - 1 > i - 4 && values[i - 4] != "d")
                        {
                            // we might get the "Index out of range" exception
                            try
                            {
                                property.Value = parts[8].Split(',')[Int32.Parse(values[i - 4])];
                            }
                            catch (Exception) { };
                        }
                    }

                    main.parameters.Item.Add(property);
                }

                main.parameters.Refresh();
            }

            this.mode = Mode.Idle;

            this.ExecuteScheduledCheck();
        }

        public void Terminate(){
            this.process.Kill();
        }

        public void ShowLastErrorInfo()
        {
            Main main = Main.Get();

            main.WriteToConsole("Last error info:");
            main.WriteToConsole(this.lastCheckContent);
        }

        public string GetParamString()
        {
            string str = "";

            foreach (PropertyGridEx.CustomProperty param in Main.Get().parameters.Item)
            {
                // default value is being used
                if (param.Value == param.DefaultValue)
                {
                    str += "d ";
                }
                // integer type
                else if (param.Type.Name == "Int32")
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

        public void Benchmark()
        {
            Main main = Main.Get();

            string benchScript = System.IO.File.ReadAllText("../examples/basic.nut");

            string[] parameters = { "1024 1024", "2048 2048", "4096 4096" };

            int totalPhases = 2 * parameters.Length;

            // the benchmark is starting right now
            if (this.benchmarkStatus == null)
            {
                this.benchmarkStatus = new BenchmarkStatus();

                main.WriteToConsole(Environment.NewLine + "Starting benchmark..." + Environment.NewLine);

                this.ExecuteScript(benchScript, false, parameters[0]);

                main.WriteToConsole("Starting reference script...");

                this.benchmarkForm.progressBar.Maximum = totalPhases;

                this.benchmarkStatus.phaseStartTime = System.DateTime.Now.Ticks / 10000;
            }
            // the benchmark is already running
            else
            {
                this.mode = Mode.Idle;

                // bad exit code -> stop
                if (this.process.ExitCode != 0)
                {
                    main.WriteToConsole("Benchmark failed!");

                    if (this.benchmarkForm != null)
                    {
                        this.benchmarkStatus = null;

                        this.benchmarkForm.result.Text = "Benchmark failed!";
                        this.benchmarkForm.result.ForeColor = System.Drawing.Color.Red;
                        this.benchmarkForm.result.Visible = true;
                        this.benchmarkForm.button.Text = "Close";
                        this.benchmarkForm.progressBar.Visible = false;
                    }

                    main.ButtonsNoRunMode();

                    return;
                }

                Int64 time = System.DateTime.Now.Ticks / 10000 - this.benchmarkStatus.phaseStartTime;

                // the becnhmark script was executed
                if (this.benchmarkStatus.phase % 2 == 0)
                {
                    this.benchmarkStatus.thisBenchTime = time;

                    if (++this.benchmarkStatus.phase < totalPhases)
                    {
                        main.WriteToConsole("Starting reference script...");
                        this.ExecuteScript(main.GetScript(), false, parameters[this.benchmarkStatus.phase / 2]);
                    }
                }
                // the user script was executed
                else
                {
                    this.benchmarkStatus.sumScores += (Double) time / (Double) this.benchmarkStatus.thisBenchTime;

                    if (++this.benchmarkStatus.phase < totalPhases)
                    {
                        main.WriteToConsole("Starting benchmarked script...");
                        this.ExecuteScript(benchScript, false, parameters[this.benchmarkStatus.phase / 2]);
                    }
                }

                this.benchmarkForm.progressBar.Value = this.benchmarkStatus.phase;

                this.benchmarkStatus.phaseStartTime = System.DateTime.Now.Ticks / 10000;

                // benchmark is finished
                if (this.benchmarkStatus.phase == totalPhases)
                {
                    Double score = Math.Round(this.benchmarkStatus.sumScores / parameters.Length, 2);

                    main.WriteToConsole("Benchmark finished with score " + score.ToString() + "x");

                    this.benchmarkStatus = null;

                    this.benchmarkForm.result.Text = "Benchmark Score: " + score.ToString() + "x";
                    this.benchmarkForm.result.Visible = true;
                    this.benchmarkForm.button.Text = "Close";
                    this.benchmarkForm.progressBar.Visible = false;

                    main.ButtonsNoRunMode();
                }
            }

            /*
            for (int round = 0; round < numRounds; round++)
            {
                foreach (string paramStr in parameters)
                {
                    Int64 start = System.DateTime.Now.Ticks;

                    this.ExecuteScript(benchScript, false, paramStr);
                }
            }*/
        }
    }
}
