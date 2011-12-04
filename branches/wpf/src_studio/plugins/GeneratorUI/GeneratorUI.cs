namespace GeoGen.Studio.PlugIns
{
	using System.Windows.Input;
	using GeoGen.Studio.PlugInLoader;
	using GeoGen.Studio.PlugIns.Extensions;
	using GeoGen.Studio.PlugIns.Interfaces;
	using GeoGen.Studio.PlugIns.MenuBars;
	using GeoGen.Studio.Utilities;
	using GeoGen.Studio.Utilities.Context;

	public sealed class GeneratorUI : GeoGen.Studio.Utilities.PlugInBase.ObjectBase
	{
		private readonly ICommand startCommand;
		private readonly ICommand abortCommand;

		private readonly Context executingContext = new Context("Executing");

		private IEditor editor;
		private IGenerator generator;

		/// <summary>
		/// Initializes a new instance of the <see cref="GeneratorUI"/> class.
		/// </summary>
		public GeneratorUI()
		{
			this.startCommand = new RelayCommand(
				param => this.Start(),
				param => this.IsReady);

			this.abortCommand = new RelayCommand(
				param => this.Abort(),
				param => !this.IsReady);
		}

		/// <summary>
		/// Gets a value indicating whether the generator is ready.
		/// </summary>
		/// <value><c>true</c> if the generator is ready; otherwise, <c>false</c>.</value>
		public bool IsReady
		{
			get
			{
				return this.generator.IsReady;
			}
		}

		/// <summary>
		/// Subscribes to generator and editor events.
		/// </summary>
		/// <param name="editor">The editor.</param>
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
					ContextManager.EnterContext(this.executingContext);
				}
			};

			generator.Aborted += delegate
			{
				ContextManager.LeaveContext(this.executingContext);
			};

			generator.Failed += delegate
			{
				ContextManager.LeaveContext(this.executingContext);
			};

			generator.Finished += delegate
			{
				ContextManager.LeaveContext(this.executingContext);
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
			var generatorMenu = new MenuEntry(
				header: "Generator",
				priority: -4,
				items: new MenuEntryObservableCollection
				{
					new MenuEntry(
						header: "Execute",
						priority: 10,
						inputGestureText: "F5",
						command: this.startCommand),
					new MenuEntry(
						header: "Abort",
						priority: 9,
						inputGestureText: "F6",
						command: this.abortCommand),
				});

			menuBar.AddMenu(generatorMenu);
		}

		[OptionalRegistrator]
		public void Register(IEditor editor, IGenerator generator, IApplicationStatusDisplay applicationStatusDisplay)
		{
			applicationStatusDisplay.RegisterApplicationStatusContext(this.executingContext);
		}

		private void Start()
		{
			this.generator.Start(this.editor.Text);
		}

		private void Abort()
		{
			this.generator.Abort();
		}        
	}
}
