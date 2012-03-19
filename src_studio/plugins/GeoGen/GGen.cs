namespace GeoGen.Studio.PlugIns
{
	using System;
	using System.Collections.Generic;
	using System.Collections.ObjectModel;
	using System.ComponentModel;
	using System.Diagnostics.Contracts;
	using System.Linq;
	using System.Threading;
	using System.Windows;
	using System.Windows.Threading;

	using GeoGen.Studio;
	using GeoGen.Studio.PlugInLoader;
	using GeoGen.Studio.PlugIns.Interfaces;
	using GeoGen.Studio.Utilities.Messaging;

	internal enum TaskType{
		None,
		HeaderOnly,
		Full,
	}

	public delegate void GenerationStartingEventHandler(object sender, GenerationStartingEventArgs args);
	public delegate void GenerationStartedEventHandler(object sender, GenerationStartedEventArgs args);    
	public delegate void GenerationFailedEventHandler(object sender, GenerationFailedEventArgs args);
	public delegate void GenerationFinishedEventHandler(object sender, GenerationFinishedEventArgs args);

	public class GGen : GeoGen.Studio.Utilities.PlugInBase.ObjectBase, IGenerator, IDisposable
	{
		protected bool disposed = false;

		// The GeoGen instance.
		protected GeoGen.Net.Generator generator = new GeoGen.Net.Generator();

		// The generator will be running in a separate thread (so it doesn't block the UI)
		protected Thread thread;

		// it the worker thread allowed to continue (true = request to stop)
		protected bool killWorkerThread = false; 

		// Maps will be put into this list until the generation is completed, then this ilist will replace the Maps list.
		protected ObservableCollection<HeightData> temporaryMapList = new ObservableCollection<HeightData>();
		
		public event GenerationStartingEventHandler Starting;
		public event GenerationStartedEventHandler Started;
		public event EventHandler HeaderLoaded;
		public event EventHandler Aborted;
		public event GenerationFailedEventHandler Failed;
		public event GenerationFinishedEventHandler Finished;        

		private TaskType CurrentTaskType { get; set; }
		
		public bool IsReady{
			get{
				// Header-scans have lower priority (and will be killed if a full request comes).
				return this.CurrentTaskType != TaskType.Full;
			}
		}

		public double? Progress {get; protected set;}

		public ObservableCollection<HeightData> Maps {get; protected set;}

		public ObservableCollection<ScriptArg> Args { get; protected set; }

		public GGen(){
			this.CurrentTaskType = TaskType.None;
			this.Maps = new ObservableCollection<HeightData>();
			this.Args = new ObservableCollection<ScriptArg>();

			this.generator.MessageThrown += delegate(object o, GeoGen.Net.MessageEventArgs args)
			{
				// GGenNet message must be converted into the Studio format
				string text = args.Message;

				if(args.Line != -1){
					text += " on line " + args.Line + "." ;
				}

				MessageType type = MessageType.Message;

				switch(args.Level){
					case GeoGen.Net.MessageLevel.Error: type = MessageType.Error; break;
					case GeoGen.Net.MessageLevel.Warning: type = MessageType.Warning; break;
					case GeoGen.Net.MessageLevel.Notice:
					case GeoGen.Net.MessageLevel.Message: type = MessageType.Message; break;
				}

				
				this.ThrowMessage(new Message(text, type));				
			};

			this.generator.ProgressChanged += delegate(object o, GeoGen.Net.ProgressEventArgs args)
			{                
				this.Progress = (double)args.CurrentProgress / (double)args.MaximumProgress;
			};

			this.generator.MapReturned += delegate(object o, GeoGen.Net.MapReturnedEventArgs args)
			{
				this.AddHeightDataToTemporaryMapList(args.Label, args.HeightMap);
			};
		}

		public void Register() {}

		/*public void Register(IQuickActionDisplay quickActionDisplay)
		{
			
		}

		public void Register(IMenuBar menuBar)
		{
			
		}

		public void Register(IMainWindowToolBar toolBar)
		{
			
		}*/

		[OptionalRegistrator]
		public void Register(IProgressDisplay progressBar)
		{
			// Register window menu entries
			//generator.

			//menuBar.AddMenu(generatorMenu);

			this.PropertyChanged += delegate(object o, PropertyChangedEventArgs args)
			{
				if (args.PropertyName != "Progress")
				{
					return;
				}

				progressBar.SetProgress("Map generation", this.Progress);
			};
		}

		public void Start(string script, bool headerOnly = false, IEnumerable<uint> parametersOverride = null)
		{
			// Event handlers may prevent the generator from continuing.
			if(!this.OnStarting(script, headerOnly))
			{
				return;
			}
			
			// Two GeoGens can't be running concurrently.
			if(!headerOnly && this.CurrentTaskType == TaskType.Full)
			{
				throw new InvalidOperationException("The generator is already running in full mode.");
			}
			else if(!headerOnly && this.CurrentTaskType == TaskType.HeaderOnly)
			{
				this.Abort();
			}
			else if (headerOnly && this.CurrentTaskType != TaskType.None)
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

			this.OnStarted(script, headerOnly);

			this.killWorkerThread = false;
			System.Threading.ThreadStart starter = new System.Threading.ThreadStart(delegate()
			{
				// Compile the script.
				try
				{
					this.generator.SetScript(script);
				}
				catch(GeoGen.Net.SyntaxErrorException e)
				{
					this.OnFailed(e.Message, false);

					return;
				}

				// Load script header.
				try
				{
					this.generator.LoadArgs();
				}
				catch (GeoGen.Net.ArgsUnreadableException e)
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

				if(headerOnly){
					this.Reset();
					return;
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

					if (this.killWorkerThread) return;
				}
				catch (GeoGen.Net.GenerationFailedException)
				{
					if (this.killWorkerThread) return;

					this.OnFailed("Map generation failed!", true);

					return;
				}        

				this.AddHeightDataToTemporaryMapList("[Main]", result);

				this.OnFinished(new TimeSpan(System.DateTime.Now.Ticks - timeStartedInTicks));
			});

			this.thread = new Thread(starter);
			this.thread.Start();
		}

		public void Abort()
		{
			this.Reset();            
			this.OnAborted();
		}

		protected void Reset()
		{
			this.killWorkerThread = true;
			this.thread = null;
			this.CurrentTaskType = TaskType.None;
		}

		protected uint[] ExportArgsFromGeneratorToValues()
		{
			uint[] values = new uint[this.Args.Count];

			int i = 0;
			foreach(ScriptArg arg in this.Args)
			{
				values[i] = arg.Value;
				i++;
			}

			return values;
		}

		protected void ExportArgsFromGenerator()
		{
			if(this.generator.Args == null)
			{
				// The args were not loaded from the generator yet;
				this.Args.Clear();  
				return;
			}

			Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal, (Action)delegate()
			{
				uint[] oldValues = new uint[this.Args.Count];

				int i = 0;
				foreach(ScriptArg arg in this.Args)
				{
					oldValues[i] = arg.Value;
					i++;
				}

				this.Args.Clear();  

				i = 0;
				foreach (GeoGen.Net.ScriptArg geoGenArg in this.generator.Args)
				{
					ScriptArg extendedArg = new ScriptArg(geoGenArg);
					if (i < oldValues.Length)
					{
						extendedArg.Value = oldValues[i];
					}
					this.Args.Add(extendedArg);
					
					i++;
				}
			});
		}

		protected void ImportArgsIntoGenerator()
		{
			/*int i = 0;
			foreach(ScriptArg arg in this.Args)
			{                
				if (i == this.generator.Args.Count())
				{
					// We ran out of parameters...
					break;
				}

				if (arg.Type == this.generator.Args[i].Type)
				{
					this.generator.Args[i].Value = arg.Value;
				}

				i++;
			}*/

			Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal, (Action)delegate()
			{
				var zipped = this.Args.Zip(this.generator.Args, (LocalArg, GeneratorArg) => new { LocalArg, GeneratorArg});

				foreach (var item in zipped)
				{
					if (item.LocalArg.Type == item.GeneratorArg.Type)
					{
						item.GeneratorArg.Value = item.LocalArg.Value;
					}
				}
			});
		}

		protected void ImportArgsIntoGeneratorFromValues(IEnumerable<uint> values)
		{
			var zipped = this.generator.Args.Zip(values, (Arg, Value) => new {Arg, Value});

			foreach(var item in zipped){
				item.Arg.Value = item.Value;
			}

			/*foreach(uint value in values)
			{
				if (i == this.generator.Args.Count())
				{
					// We ran out of parameters...
					break;
				}

				this.generator.Args[i].Value = value;
				
				i = 0;
			}*/
		}        

		protected bool OnStarting(string script, bool headerOnly)
		{
			GenerationStartingEventArgs args = new GenerationStartingEventArgs(script, headerOnly);

			Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal, (Action)delegate()
			{          
				if (this.Starting != null)
				{                
					this.Starting(this, args);
				}
			});

			return args.Continue;
		}

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

		protected void OnAborted()
		{
			

			Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal, (Action)delegate()
			{
				this.ThrowMessage(
					new Message("Generation aborted!", MessageType.Message)
				);

				// Terminate the generator thread and put the generator back into ready state.
				this.Reset();

				if (this.Aborted != null)
				{
					this.Aborted(this, new EventArgs());
				}
			});
		}

		protected void OnFailed(string message, bool isHeaderLoaded){
			Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal, (Action)delegate()
			{
				this.ThrowMessage(
					new Message(message, MessageType.Error)
				);

				// Terminate the generator thread and put the generator back into ready state.
				this.Reset();

				if (this.Failed != null)
				{
					this.Failed(this, new GenerationFailedEventArgs(message, isHeaderLoaded));
				}
			});         
		}

		protected void OnFinished(TimeSpan timeSpan)
		{
			Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal, (Action)delegate()
			{
				this.Progress = null;
				this.Maps.Clear();                
				//this.temporaryMapList.CopyTo(this.Maps, 0);
				
				foreach(HeightData heightData in this.temporaryMapList)
				{
					this.Maps.Add(heightData);
				}
				
				this.temporaryMapList.Clear();

				this.ThrowMessage(
					new Message("Map generated after " + timeSpan.ToString() + ".", MessageType.Message)
				);

				// Terminate the generator thread and put the generator back into ready state.
				this.Reset();

				if (this.Finished != null)
				{
					this.Finished(this, new GenerationFinishedEventArgs(timeSpan));
				}
			});
		}

		protected void AddHeightDataToTemporaryMapList(string name, GeoGen.Net.HeightData heightData)
		{
			Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal, (Action) delegate
			{
				this.temporaryMapList.Add(
					new HeightData(name, heightData)
				);
			});
		}

		private void ThrowMessage(Message message)
		{
			if (this.CurrentTaskType == TaskType.HeaderOnly)
			{
				return;
			}

			Messenger.ThrowMessage(message);
		}

		void IDisposable.Dispose()
		{			
			this.Dispose(true);

			GC.SuppressFinalize(this);
		}

		~GGen()
		{
			this.Dispose(false);
		}

		protected virtual void Dispose(bool disposing)
		{
			if (this.disposed)
			{
				return;
			}

			this.generator.Dispose();
		}
	}
}
