using GeoGen.Net;
using GeoGen.Studio;
using GeoGen.Studio.Utilities.Messaging;
using System.Diagnostics.Contracts;
using System.Collections.ObjectModel;
using System.Threading;
using System.Windows.Threading;
using System;
using System.Collections.Generic;

namespace GeoGen.Studio.PlugIns
{
    internal enum TaskType{
        None,
        HeaderOnly,
        Full,
    }
    
    public delegate void GenerationFailedEventHandler(object sender, GenerationFailedEventArgs args);
    public delegate void GenerationFinishedEventHandler(object sender, GenerationFinishedEventArgs args);

    public class GGen : GeoGen.Studio.Utilities.PlugInBase.Object, IGenerator
    {
        // The GeoGen instance.
        protected Generator generator = new Generator();

        protected Thread thread;

        // Maps will be put into this list until the generation is completed, then this ilist will replace the Maps list.
        protected ObservableCollection<HeightData> temporaryMapList = new ObservableCollection<HeightData>();
        
        public event GenerationFailedEventHandler GenerationFailed;
        public event GenerationFinishedEventHandler GenerationFinished;
        public event EventHandler HeaderLoaded;

        private TaskType CurrentTaskType { get; set; }

        public bool IsReady{
            get{
                // Header-scans have lower priority (and will be killed if a full request comes).
                return this.CurrentTaskType != TaskType.Full;
            }
        }

        public double Progress{get; protected set;}

        public ObservableCollection<HeightData> Maps {get; protected set;}

        public ObservableCollection<ScriptArg> Args { get; protected set; }

        public GGen(){
            this.Progress = 0;
            this.CurrentTaskType = TaskType.None;
            this.Maps = new ObservableCollection<HeightData>();
            this.Args = new ObservableCollection<ScriptArg>();

            this.generator.MessageThrown += delegate(object o, MessageEventArgs args)
            {
                // GGenNet message must be converted into the Studio format
                string text = args.Message;

                if(args.Line != -1){
                    text += " on line " + args.Line + "." ;
                }

                MessageType type = MessageType.Message;

                switch(args.Level){
                    case MessageLevel.Error: type = MessageType.Error; break;
                    case MessageLevel.Warning: type = MessageType.Warning; break;
                    case MessageLevel.Notice:
                    case MessageLevel.Message: type = MessageType.Message; break;
                }

                this.ThrowMessage(new Message(text, type));
            };

            this.generator.ProgressChanged += delegate(object o, ProgressEventArgs args)
            {                
                this.Progress = (double)args.CurrentProgress / (double)args.MaximumProgress;
            };

            this.generator.MapReturned += delegate(object o, MapReturnedEventArgs args)
            {
                this.AddHeightDataToTemporaryMapList(args.Label, args.HeightMap);
            };
        }

        public void Register() {}

        public void Start(string script, bool headerOnly = false, IEnumerable<uint> parametersOverride = null)
        {
            // Two GeoGens can't be running concurrently.
            if(!headerOnly && this.CurrentTaskType == TaskType.Full)
            {
                throw new InvalidOperationException("The generator is already running in full mode.");
            }
            else if(!headerOnly && this.CurrentTaskType == TaskType.HeaderOnly)
            {
                this.Abort();
            }
            else if(headerOnly && this.CurrentTaskType != null)
            {
                // It is not necessary to scan headers two times at once
                return;
            }

            if(headerOnly)
            {
                this.CurrentTaskType = TaskType.HeaderOnly;
            }
            else
            {
                this.CurrentTaskType = TaskType.Full;
                this.Maps.Clear();

                this.ThrowMessage(new Message("Generation started.", MessageType.Message));
            }

            System.Threading.ThreadStart starter = new System.Threading.ThreadStart(delegate()
            {
                // Compile the script.
                try
                {
                    this.generator.SetScript(script);
                }
                catch(SyntaxErrorException e)
                {
                    this.OnGenerationFailed(e.Message, false);

                    return;
                }

                // Load script header.
                try
                {
                    this.generator.LoadArgs();
                }
                catch (ArgsUnreadableException e)
                {
                    this.OnGenerationFailed(e.Message, false);

                    return;
                }

                this.OnHeaderLoaded();

                if(headerOnly){
                    return;
                }

                // Update the argument values in GeoGen.
                if(parametersOverride != null)
                {
                    this.ImportArgsIntoGenerator();
                }
                else
                {
                    this.ImportArgsIntoGeneratorFromValues(parametersOverride);
                }

                Int64 timeStartedInTicks = System.DateTime.Now.Ticks;

                // Generate the map.
                GeoGen.Net.HeightData result;
                try
                {
                    /*if (this.config.seed == 0)
                    {
                        ggen.Seed = (uint)DateTime.Now.Ticks;
                    }
                    else
                    {
                        ggen.Seed = this.config.seed;
                    }*/

                    result = this.generator.Generate();
                }
                catch (GenerationFailedException)
                {
                    this.OnGenerationFailed("Map generation failed!", true);

                    return;
                }

                this.AddHeightDataToTemporaryMapList("[Main]", result);

                this.OnGenerationFinished(new TimeSpan(System.DateTime.Now.Ticks - timeStartedInTicks));
            });

            this.thread.Start(starter);
        }

        public void Abort()
        {
            this.thread.Abort();
            this.thread = null;
            this.CurrentTaskType = TaskType.None;
        }

        protected void ExportArgsFromGenerator()
        {
            this.Args.Clear();

            foreach(GeoGen.Net.ScriptArg geoGenArg in this.generator.Args)
            {
                this.Args.Add(new ScriptArg(geoGenArg));
            }
        }

        protected void ImportArgsIntoGenerator()
        {
            int i = 0;
            foreach(ScriptArg arg in this.Args)
            {                
                if (i == this.generator.Args.Length)
                {
                    // We ran out of parameters...
                    break;
                }

                if (arg.Type == this.generator.Args[i].Type)
                {
                    this.generator.Args[i].Value = arg.Value;
                }

                i++;
            }
        }

        protected void ImportArgsIntoGeneratorFromValues(IEnumerable<uint> values)
        {
            int i = 0;
            foreach(uint value in values)
            {
                if (i == this.generator.Args.Length)
                {
                    // We ran out of parameters...
                    break;
                }

                this.generator.Args[i].Value = value;
                
                i = 0;
            }
        }

        protected void OnGenerationFailed(string message, bool isHeaderLoaded){
            Dispatcher.CurrentDispatcher.Invoke(DispatcherPriority.Normal, (Action) delegate()
            {
                // Terminate the generator thread and put the generator back into ready state.
                this.Abort();

                this.ThrowMessage(
                    new Message(message, MessageType.Error)
                );

                this.GenerationFailed(this, new GenerationFailedEventArgs(message, isHeaderLoaded));
            });         
        }

        protected void OnGenerationFinished(TimeSpan timeSpan)
        {
            Dispatcher.CurrentDispatcher.Invoke(DispatcherPriority.Normal, (Action)  delegate()
            {
                this.Progress = 0;

                // Terminate the generator thread and put the generator back into ready state.
                this.Abort();

                this.ThrowMessage(
                    new Message("Map generated after " + timeSpan.ToString() + ".", MessageType.Message)
                );

                this.GenerationFinished(this, new GenerationFinishedEventArgs(timeSpan));
            });
        }

        protected void OnHeaderLoaded()
        {
            Dispatcher.CurrentDispatcher.Invoke(DispatcherPriority.Normal, (Action)delegate()
            {
                this.HeaderLoaded(this, new EventArgs());
            });
        }

        protected void AddHeightDataToTemporaryMapList(string name, GeoGen.Net.HeightData heightData)
        {
            this.temporaryMapList.Add(
                new HeightData(name, heightData)
            );
        }

        [ContractInvariantMethod]   
        private void ObjectInvariant()
        {
            // Progress percentage must be in range [0, 100].
            Contract.Assert(this.Progress >= 0 && this.Progress <= 100);
            Contract.Assert(
                (this.CurrentTaskType == TaskType.None && this.thread == null) ||
                (this.CurrentTaskType != TaskType.None && this.thread != null)
            );
        }
    }
}
