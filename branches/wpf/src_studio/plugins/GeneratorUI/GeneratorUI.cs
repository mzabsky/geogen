namespace GeoGen.Studio.PlugIns
{
	using System;
	using System.Windows.Input;
	using GeoGen.Studio.PlugInLoader;
	using GeoGen.Studio.PlugIns.Interfaces;
	using GeoGen.Studio.PlugIns.MenuBars;
	using GeoGen.Studio.Utilities;
	using GeoGen.Studio.Utilities.Context;

	class GeneratorUI: GeoGen.Studio.Utilities.PlugInBase.ObjectBase
	{
		protected Context executingContext = new Context("Executing");
		protected IEditor editor;
		protected IGenerator generator;

		protected ICommand startCommand;
		protected ICommand abortCommand;

		/// <summary>
		/// Gets a value indicating whether the generator is ready.
		/// </summary>
		/// <value><c>true</c> if the generator is ready; otherwise, <c>false</c>.</value>
		protected bool IsReady
		{
			get
			{
				return generator.IsReady;
			}
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="GeneratorUI"/> class.
		/// </summary>
		public GeneratorUI()
		{
			this.startCommand = new RelayCommand(
				param => this.Start(),
				param => this.IsReady
			);

			this.abortCommand = new RelayCommand(
				param => this.Abort(),
				param => !this.IsReady
			);
		}

		/// <summary>
		/// Subscribes to generator and editor events.
		/// </summary>
		/// <param name="generator">The generator.</param>
		[OptionalRegistrator]
		public void Register(IEditor editor, IGenerator generator)
		{
			this.generator = generator;
			this.editor = editor;

			generator.Started += delegate(object o, GenerationStartedEventArgs args)
			{
				if (!args.HeaderOnly)
				{
					ContextManager.EnterContext(executingContext);
				}
			};

			generator.Aborted += delegate(object o, EventArgs args)
			{
				ContextManager.LeaveContext(executingContext);
			};

			generator.Failed += delegate(object o, GenerationFailedEventArgs args)
			{
				ContextManager.LeaveContext(executingContext);
			};

			generator.Finished += delegate(object o, GenerationFinishedEventArgs args)
			{
				ContextManager.LeaveContext(executingContext);
			};
		}

		[OptionalRegistrator]
		public void Register(IEditor editor, IGenerator generator, IMainWindow mainWindow)
		{
			// Register hotkeys
			mainWindow.RegisterInputGesture(new KeyGesture(Key.F5), this.startCommand);
			mainWindow.RegisterInputGesture(new KeyGesture(Key.F6), this.abortCommand);
		}

		[OptionalRegistrator]
		public void Register(IEditor editor, IGenerator generator, IMenuBar menuBar)
		{
			// Register window menu entries
			MenuEntry generatorMenu = new MenuEntry(
				header: "Generator",
				priority: -4,
				items: new MenuEntryObservableCollection()
				{
					new MenuEntry(
						header: "Execute",
						priority: 10,
						inputGestureText: "F5",
						command: this.startCommand
					),
					new MenuEntry(
						header: "Abort",
						priority: 9,
						inputGestureText: "F6",
						command: this.abortCommand
					),
				}
			);

			menuBar.AddMenu(generatorMenu);
		}

		[OptionalRegistrator]
		public void Register(IEditor editor, IGenerator generator, IApplicationStatusDisplay applicationStatusDisplay)
		{
			applicationStatusDisplay.RegisterApplicationStatusContext(executingContext);
		}

		protected void Start()
		{
			this.generator.Start(this.editor.Text);
		}

		protected void Abort()
		{
			this.generator.Abort();
		}        
	}
}
