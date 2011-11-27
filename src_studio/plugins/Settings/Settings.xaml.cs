namespace GeoGen.Studio.PlugIns
{
	using System.ComponentModel;
	using System.Collections.Generic;
	using System.Linq;
	using System.Windows.Input;
	using GeoGen.Studio.PlugInLoader;
	using GeoGen.Studio.PlugIns.Interfaces;
	using GeoGen.Studio.PlugIns.MenuBars;
	using GeoGen.Studio.Utilities;    
	using GeoGen.Studio.Utilities.Configurability;

	public sealed partial class Settings : GeoGen.Studio.Utilities.PlugInBase.WindowBase, INotifyPropertyChanged
	{
		private readonly ICommand showCommand;
		private readonly ICommand applyCommand;
		private readonly ICommand closeCommand;
		private readonly ICommand okCommand;
		private readonly ICommand togglePlugInCommand;

		public ICommand ShowCommand {
			get
			{
				return this.showCommand;
			}
		}

		public ICommand ApplyCommand {
			get
			{
				return this.applyCommand;
			}
		}

		public ICommand CloseCommand {
			get
			{
				return this.closeCommand;
			}
		}

		public ICommand OkCommand
		{
			get
			{
				return this.okCommand;
			}
		}

		public ICommand TogglePlugInCommand
		{
			get
			{
				return this.togglePlugInCommand;
			}
		}

		public IEnumerable<PlugInInfo> PlugIns {get; private set;}

		[Configurable(DefaultValue = false, EnableVisualConfiguration = false)]
		public bool ShowHiddenPlugIns { get; set; }

		/// <summary>
		/// Initializes a new instance of the <see cref="Settings"/> class.
		/// </summary>
		public Settings()
		{
			this.PropertyChanged += delegate(object o, PropertyChangedEventArgs args)
			{
				if (args.PropertyName == "ShowHiddenPlugIns")
				{
					bool a = this.ShowHiddenPlugIns;
					int i = 0;
				}
			};
			
			ICommand command = this.ShowCommand;

			this.showCommand = new RelayCommand(
				param => this.ShowWindow()
			);

			this.applyCommand = new RelayCommand(
				param => this.ApplyChanges()
			);

			this.closeCommand = new RelayCommand(
				param => this.Hide()
			);

			this.okCommand = new RelayCommand(
				param =>
				{
					this.ApplyChanges(); 
					this.Hide();
				}
			);

			this.togglePlugInCommand = new RelayCommand(
				param =>
				{
					(param as PlugInInfo).IsEnabled = !(param as PlugInInfo).IsEnabled;
					int i = 0;
				}
			);

			this.Closing += delegate(object sender, CancelEventArgs args)
			{
				args.Cancel = true;
				this.Hide();
			};

			this.PlugIns = Enumerable.Empty<PlugInInfo>();

			InitializeComponent();

			MainConfig.Register(this);
		}

		/// <summary>
		/// Adds a button to the main menu bar.
		/// </summary>
		/// <param name="menuBar">The menu bar.</param>
		public void Register(IMenuBar menuBar)
		{
			menuBar.AddMenu(
				new MenuEntry(
					header: "Edit",
					priority: -1,
					items: new MenuEntryObservableCollection {
						new MenuSeparator(
							priority: -31
						),
						new MenuEntry(
							header: "Settings",                            
							priority: -32,
							command: this.showCommand,
							icon: "pack://application:,,,/GGenStudio.PlugIn.Settings;component/Images/Icons/settings.png"
						)
					}
				)
			);
		}

		private void ShowWindow()
		{            
			List<PlugInInfo> list = new List<PlugInInfo>();

			bool a = this.ShowHiddenPlugIns;

			foreach(PlugIn plugIn in Loader.PlugIns)
			{               
				MainConfig.SaveConfiguration(plugIn.Instances.Last());                

				list.Add(new PlugInInfo(plugIn));
			}

			this.PlugIns = list.OrderBy(p => p.PlugIn.Name);            

			this.ShowDialog();
		}

		private void ApplyChanges()
		{
			foreach(PlugInInfo plugInInfo in this.PlugIns)
			{
				// Update enabled status
				plugInInfo.PlugIn.IsEnabled = plugInInfo.IsEnabled;

				// Do not apply configuration changes for non-configurable plug-ins              
				foreach(ConfigurablePropertyInfo property in plugInInfo.Properties)
				{
					object o = property.Value;

					MainConfig.SavePropertyValue(property.Property, property.Value);

					foreach(object instance in plugInInfo.PlugIn.Instances)
					{
						property.Property.SetValue(instance, property.Value, null);
					}                    
				}
			}
		}

		private void TogglePlugInEnabled(PlugInInfo plugInInfo)
		{
			plugInInfo.IsEnabled = !plugInInfo.IsEnabled;
		}
	}
}
