namespace GeoGen.Studio.PlugIns
{
    using System;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using System.ComponentModel;
    using System.Linq;
    using System.Threading;
    using System.Windows;
    using System.Windows.Threading;

    using GeoGen.Net;
    using GeoGen.Studio.PlugInLoader;
    using GeoGen.Studio.PlugIns.Interfaces;
    using GeoGen.Studio.Utilities.Messaging;

    using HeightData = GeoGen.Studio.HeightData;
    using ScriptArg = GeoGen.Studio.ScriptArg;

    /// <summary>
    /// Type of currently executed generator task.
    /// </summary>
    internal enum TaskType
    {
        /// <summary>
        /// No task is being executed.
        /// </summary>
        None,

        /// <summary>
        /// Script valitadion (only script header) is being executed.
        /// </summary>
        HeaderOnly,

        /// <summary>
        /// Whole script is being executed.
        /// </summary>
        Full,
    }

    /// <summary>
    /// Encapsulates GeoGen as a plug-in.
    /// </summary>
    public class GGen : GeoGen.Studio.Utilities.PlugInBase.ObjectBase, IGenerator, IDisposable
    {
        /// <summary>
        /// Actual generator instance.
        /// </summary>
        private readonly GeoGen.Net.Generator generator = new GeoGen.Net.Generator();

        /// <summary>
        /// Maps will be put into this list until the generation is completed, then this list will replace the <see cref="Maps"/> list.
        /// </summary>
        private readonly ObservableCollection<HeightData> temporaryMapList = new ObservableCollection<HeightData>();

        /// <summary>
        /// True if this object was already <see cref="IDisposable">disposed</see>.
        /// </summary>
        private bool disposed;

        /// <summary>
        /// The generator will be running in a separate generatorThread (so it doesn't block the UI)
        /// </summary>
        private Thread generatorThread;

        /// <summary>
        /// True if the generator thread was requested to stop next time it returns to managed code.
        /// </summary>
        private bool killWorkerThread;

        /// <summary>
        /// Initializes a new instance of the <see cref="GGen"/> class.
        /// </summary>
        public GGen()
        {
            this.disposed = false;
            this.CurrentTaskType = TaskType.None;
            this.Maps = new ObservableCollection<HeightData>();
            this.Args = new ObservableCollection<ScriptArg>();

            this.generator.MessageThrown += delegate(object o, GeoGen.Net.MessageEventArgs args)
            {
                // GGenNet message must be converted into the Studio format
                string text = args.Message;

                if (args.Line != -1)
                {
                    text += " on line " + args.Line + ".";
                }

                MessageType type = MessageType.Message;

                switch (args.Level)
                {
                    case GeoGen.Net.MessageLevel.Error:
                        type = MessageType.Error;
                        break;
                    case GeoGen.Net.MessageLevel.Warning:
                        type = MessageType.Warning;
                        break;
                    case GeoGen.Net.MessageLevel.Notice:
                    case GeoGen.Net.MessageLevel.Message:
                        type = MessageType.Message;
                        break;
                }

                this.ThrowMessage(new Message(text, type));                
            };

            this.generator.ProgressChanged += delegate(object o, GeoGen.Net.ProgressEventArgs args)
            {                
                this.Progress = args.CurrentProgress / args.MaximumProgress;
            };

            this.generator.MapReturned += delegate(object o, GeoGen.Net.MapReturnedEventArgs args)
            {
                this.AddHeightDataToTemporaryMapList(args.Label, args.HeightMap);
            };
        }

        /// <summary>
        /// Finalizes an instance of the GGen class. Releases unmanaged resources and performs other cleanup operations before the
        /// <see cref="GGen"/> is reclaimed by garbage collection.
        /// </summary>
        ~GGen()
        {
            this.Dispose(false);
        }

        /// <summary>
        /// Occurs before the generator starts.
        /// </summary>
        public event EventHandler<GenerationStartingEventArgs> Starting;

        /// <summary>
        /// Occurs after the generator starts.
        /// </summary>
        public event EventHandler<GenerationStartedEventArgs> Started;

        /// <summary>
        /// Occurs when the header is successfully loaded.
        /// </summary>
        public event EventHandler HeaderLoaded;

        /// <summary>
        /// Occurs when the generation is aborted by user.
        /// </summary>
        public event EventHandler Aborted;

        /// <summary>
        /// Occurs when the generation failed (and it was not aborted).
        /// </summary>
        public event EventHandler<GenerationFailedEventArgs> Failed;

        /// <summary>
        /// Occurs when the generation was successfully finished.
        /// </summary>
        public event EventHandler<GenerationFinishedEventArgs> Finished;

        /// <summary>
        /// Gets a value indicating whether the generator is ready to accept requests to run scripts.
        /// </summary>
        /// <value>
        ///   <c>true</c> if the generator is ready; otherwise, <c>false</c>.
        /// </value>
        public bool IsReady
        {
            get
            {
                // Header-scans have lower priority (and will be killed if a full request comes).
                return this.CurrentTaskType != TaskType.Full;
            }
        }

        /// <summary>
        /// Gets or sets a value indicating the progress within current map script in percents. The value is 0 if no full script is running.
        /// </summary>
        /// <value>
        /// The progress.
        /// </value>
        public double? Progress { get; protected set; }

        /// <summary>
        /// Gets or sets a collecion of maps generated by last generator run.
        /// </summary>
        public ObservableCollection<HeightData> Maps { get; protected set; }

        /// <summary>
        /// Gets or sets a collection of args which will be used by current script. Fills in default values when the script is first loaded.
        /// </summary>
        public ObservableCollection<ScriptArg> Args { get; protected set; }

        /// <summary>
        /// Gets or sets the type of the current task.
        /// </summary>
        /// <value>
        /// The type of the current task.
        /// </value>
        private TaskType CurrentTaskType { get; set; }

        /// <summary>
        /// Make sure the <see cref="Loader"/> creates an instance even when the optional registrators are not exeuted.
        /// </summary>
        public void Register()
        {            
        }

        /*public void Register(IQuickActionDisplay quickActionDisplay)
        {
            
        }

        public void Register(IMenuBar menuBar)
        {
            
        }

        public void Register(IMainWindowToolBar toolBar)
        {
            
        }*/

        /// <summary>
        /// Registers the generator to display its progress to all available <see cref="IProgressDisplay"/>.
        /// </summary>
        /// <param name="progressBar">The progress display.</param>
        [OptionalRegistrator]
        public void Register(IProgressDisplay progressBar)
        {
            this.PropertyChanged += delegate(object o, PropertyChangedEventArgs args)
            {
                if (args.PropertyName != "Progress")
                {
                    return;
                }

                progressBar.SetProgress("Map generation", this.Progress);
            };
        }

        /// <summary>
        /// Executed the specified script.
        /// </summary>
        /// <param name="script">The script.</param>
        /// <param name="headerOnly">if set to <c>true</c> [header only].</param>
        /// <param name="parametersOverride">The parameters override.</param>
        public void Start(string script, bool headerOnly = false, IEnumerable<uint> parametersOverride = null)
        {
            // Event handlers may prevent the generator from continuing.
            if (!this.OnStarting(script, headerOnly))
            {
                return;
            }
            
            // Two GeoGens can't be running concurrently.
            if (!headerOnly && this.CurrentTaskType == TaskType.Full)
            {
                throw new InvalidOperationException("The generator is already running in full mode.");
            }
            else if (!headerOnly && this.CurrentTaskType == TaskType.HeaderOnly)
            {
                this.Abort();
            }
            else if (headerOnly && this.CurrentTaskType != TaskType.None)
            {
                // It is not necessary to scan headers two times at once
                return;
            }

            if (headerOnly)
            {
                this.CurrentTaskType = TaskType.HeaderOnly;
            }
            else
            {
                this.CurrentTaskType = TaskType.Full;
                this.Maps.Clear();

                this.ThrowMessage(new Message("Generation started."));
            }

            this.OnStarted(script, headerOnly);

            this.killWorkerThread = false;
            ThreadStart starter = delegate
                {
                    // Compile the script.
                    try
                    {
                        this.generator.SetScript(script);
                    }
                    catch (SyntaxErrorException e)
                    {
                        this.OnFailed(e.Message, false);

                        return;
                    }

                    // Load script header.
                    try
                    {
                        this.generator.LoadArgs();
                    }
                    catch (ArgsUnreadableException e)
                    {
                        this.OnFailed(e.Message, false);

                        return;
                    }

                    this.ExportArgsFromGenerator();

                    // Update the argument values in GeoGen.
                    if (parametersOverride == null)
                    {
                        this.ImportArgsIntoGenerator();
                    }
                    else
                    {
                        this.ImportArgsIntoGeneratorFromValues(parametersOverride);
                    }

                    this.OnHeaderLoaded();

                    if (headerOnly)
                    {
                        this.Reset();
                        return;
                    }                              

                    long timeStartedInTicks = System.DateTime.Now.Ticks;

                    // Generate the map.
                    Net.HeightData result;
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

                        if (this.killWorkerThread)
                        {
                            return;
                        }
                    }
                    catch (GeoGen.Net.GenerationFailedException)
                    {
                        if (this.killWorkerThread)
                        {
                            return;
                        }

                        this.OnFailed("Map generation failed!", true);

                        return;
                    }        

                    this.AddHeightDataToTemporaryMapList("[Main]", result);

                    this.OnFinished(new TimeSpan(System.DateTime.Now.Ticks - timeStartedInTicks));
                };

            this.generatorThread = new Thread(starter);
            this.generatorThread.Start();
        }

        /// <summary>
        /// Aborts the script being executed.
        /// </summary>
        public void Abort()
        {
            this.Reset();            
            this.OnAborted();
        }

        /// <summary>
        /// Performs application-defined tasks associated with freeing, releasing, or resetting unmanaged resources.
        /// </summary>
        void IDisposable.Dispose()
        {
            this.Dispose(true);

            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Resets the generator to be ready to execute another script.
        /// </summary>
        protected void Reset()
        {
            this.killWorkerThread = true;
            this.generatorThread = null;
            this.CurrentTaskType = TaskType.None;
        }

        /// <summary>
        /// Converts studio args to value array.
        /// </summary>
        /// <returns>The converted args.</returns>
        protected uint[] ExportArgsToValues()
        {
            var values = new uint[this.Args.Count];

            int i = 0;
            foreach (ScriptArg arg in this.Args)
            {
                values[i] = arg.Value;
                i++;
            }

            return values;
        }

        /// <summary>
        /// Converts generator args to studio args.
        /// </summary>
        protected void ExportArgsFromGenerator()
        {
            if(this.generator.Args == null)
            {
                // The args were not loaded from the generator yet;
                this.Args.Clear();  
                return;
            }

            var conversion = (Action)delegate
                {
                    var oldValues = new uint[this.Args.Count];

                    int i = 0;
                    foreach (ScriptArg arg in this.Args)
                    {
                        oldValues[i] = arg.Value;
                        i++;
                    }

                    this.Args.Clear();

                    i = 0;
                    foreach (Net.ScriptArg geoGenArg in this.generator.Args)
                    {
                        var extendedArg = new ScriptArg(geoGenArg);
                        if (i < oldValues.Length)
                        {
                            extendedArg.Value = oldValues[i];
                        }
                        this.Args.Add(extendedArg);

                        i++;
                    }
                };

            Application.Current.Dispatcher.Invoke(
                DispatcherPriority.Normal, 
                conversion);
        }

        /// <summary>
        /// Converts args into generator from studio.
        /// </summary>
        protected void ImportArgsIntoGenerator()
        {
            var conversion = (Action)delegate
                {
                    var zipped = this.Args.Zip(
                        this.generator.Args, (localArg, generatorArg) => new { localArg, generatorArg });

                    foreach (var item in zipped)
                    {
                        if (item.localArg.Type == item.generatorArg.Type)
                        {
                            item.generatorArg.Value = item.localArg.Value;
                        }
                    }
                };

            Application.Current.Dispatcher.Invoke(
                DispatcherPriority.Normal,
                conversion);
        }

        /// <summary>
        /// Converts value array to studio args.
        /// </summary>
        /// <param name="values">The values.</param>
        protected void ImportArgsIntoGeneratorFromValues(IEnumerable<uint> values)
        {
            var zipped = this.generator.Args.Zip(values, (arg, value) => new {arg, value});

            foreach (var item in zipped)
            {
                item.arg.Value = item.value;
            }
        }

        /// <summary>
        /// Called before the generator starts.
        /// </summary>
        /// <param name="script">The script.</param>
        /// <param name="headerOnly">if set to <c>true</c> [header only].</param>
        /// <returns>True if the executed is requested to be terminated.</returns>
        protected virtual bool OnStarting(string script, bool headerOnly)
        {
            if (script == null)
            {
                throw new ArgumentNullException("script");
            }

            var args = new GenerationStartingEventArgs(script, headerOnly);

            Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal, (Action)delegate
            {          
                if (this.Starting != null)
                {                
                    this.Starting(this, args);
                }
            });

            return args.Continue;
        }

        /// <summary>
        /// Called after the generator starts.
        /// </summary>
        /// <param name="script">The script.</param>
        /// <param name="headerOnly">if set to <c>true</c> [header only].</param>
        protected void OnStarted(string script, bool headerOnly)
        {
            Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal, (Action)delegate()
            {
                if (this.Started != null)
                {
                    this.Started(this, new GenerationStartedEventArgs(script, headerOnly));
                }
            });
        }

        /// <summary>
        /// Called after script header was scucessfully loaded.
        /// </summary>
        protected void OnHeaderLoaded()
        {
            Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal, (Action)delegate()
            {
                if (this.HeaderLoaded != null)
                {
                    this.HeaderLoaded(this, new EventArgs());
                }
            });
        }

        /// <summary>
        /// Called when the script was aborted by user.
        /// </summary>
        protected void OnAborted()
        {
            Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal, (Action)delegate
            {
                this.ThrowMessage(new Message("Generation aborted!"));

                // Terminate the generator generatorThread and put the generator back into ready state.
                this.Reset();

                if (this.Aborted != null)
                {
                    this.Aborted(this, new EventArgs());
                }
            });
        }

        /// <summary>
        /// Called when the script failed (but was not aborted by user).
        /// </summary>
        /// <param name="message">The message.</param>
        /// <param name="isHeaderLoaded">if set to <c>true</c> [is header loaded].</param>
        protected void OnFailed(string message, bool isHeaderLoaded)
        {
            Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal, (Action)delegate
            {
                this.ThrowMessage(new Message(message, MessageType.Error));

                // Terminate the generator generatorThread and put the generator back into ready state.
                this.Reset();

                if (this.Failed != null)
                {
                    this.Failed(this, new GenerationFailedEventArgs(message, isHeaderLoaded));
                }
            });         
        }

        /// <summary>
        /// Called after the script successfully finished.
        /// </summary>
        /// <param name="timeSpan">The time span.</param>
        protected void OnFinished(TimeSpan timeSpan)
        {
            Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal, (Action)delegate()
            {
                this.Progress = null;
                this.Maps.Clear();                
                
                foreach (HeightData heightData in this.temporaryMapList)
                {
                    this.Maps.Add(heightData);
                }
                
                this.temporaryMapList.Clear();

                this.ThrowMessage(new Message("Map generated after " + timeSpan.ToString() + "."));

                // Terminate the generator generatorThread and put the generator back into ready state.
                this.Reset();

                if (this.Finished != null)
                {
                    this.Finished(this, new GenerationFinishedEventArgs(timeSpan));
                }
            });
        }

        /// <summary>
        /// Converts map from 16-bit value array and adds it to temporary map list (which is copied to the final map list
        /// when the script finishes).
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="heightData">The height data.</param>
        protected void AddHeightDataToTemporaryMapList(string name, Net.HeightData heightData)
        {
            Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal, (Action) delegate
            {
                this.temporaryMapList.Add(new HeightData(name, heightData));
            });
        }

        /// <summary>
        /// Throws a message. The messages are ignored if the generator is in header-only mode.
        /// </summary>
        /// <param name="message">The message.</param>
        protected void ThrowMessage(Message message)
        {
            if (this.CurrentTaskType == TaskType.HeaderOnly)
            {
                return;
            }

            message.Send();
        }

        /// <summary>
        /// Releases unmanaged and - optionally - managed resources
        /// </summary>
        /// <param name="disposing"><c>true</c> to release both managed and unmanaged resources; <c>false</c> to release only unmanaged resources.</param>
        protected virtual void Dispose(bool disposing)
        {
            if (this.disposed)
            {
                return;
            }

            this.generator.Dispose();
            this.disposed = true;
        }
    }
}
