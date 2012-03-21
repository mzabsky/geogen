namespace GeoGen.Studio.PlugIns
{
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Linq;
    using System.Windows.Input;

    using GeoGen.Studio.PlugInLoader;
    using GeoGen.Studio.PlugIns.Interfaces;
    using GeoGen.Studio.PlugIns.MenuBars;
    using GeoGen.Studio.Utilities;
    using GeoGen.Studio.Utilities.Persistence;

    /// <summary>
    /// The Settings dialog.
    /// </summary>
    public sealed partial class Settings : GeoGen.Studio.Utilities.PlugInBase.WindowBase, INotifyPropertyChanged
    {
        /// <summary>
        /// Backing field for <see cref="PlugIns"/> property.
        /// </summary>
        private IEnumerable<SettingsPlugInViewModel> plugIns;

        /// <summary>
        /// Initializes a new instance of the <see cref="Settings"/> class.
        /// </summary>
        public Settings()
        {
            this.ShowCommand = new RelayCommand(
                param => this.ShowWindow());

            this.ApplyCommand = new RelayCommand(
                param => this.ApplyChanges());

            this.CloseCommand = new RelayCommand(
                param => this.Hide());

            this.OkCommand = new RelayCommand(
                param =>
                {
                    this.ApplyChanges(); 
                    this.Hide();
                });

            this.TogglePlugInCommand = new RelayCommand(
                param =>
                {
                    ((SettingsPlugInViewModel)param).IsEnabled = !((SettingsPlugInViewModel)param).IsEnabled;
                });

            this.Closing += delegate(object sender, CancelEventArgs args)
            {
                args.Cancel = true;
                this.Hide();
            };

            this.PlugIns = Enumerable.Empty<SettingsPlugInViewModel>();

            InitializeComponent();

            MainConfig.Register(this);
        }

        /// <summary>
        /// Gets the show command.
        /// </summary>
        public ICommand ShowCommand { get; private set; }

        /// <summary>
        /// Gets the apply command.
        /// </summary>
        public ICommand ApplyCommand { get; private set; }

        /// <summary>
        /// Gets the close command.
        /// </summary>
        public ICommand CloseCommand { get; private set; }

        /// <summary>
        /// Gets the ok command.
        /// </summary>
        public ICommand OkCommand { get; private set; }

        /// <summary>
        /// Gets the toggle plug-in command.
        /// </summary>
        public ICommand TogglePlugInCommand { get; private set; }        

        /// <summary>
        /// Gets collection of all plug-in view models displayed within the dialog.
        /// </summary>
        public IEnumerable<SettingsPlugInViewModel> PlugIns
        {
            get
            {
                if (this.ShowHiddenPlugIns)
                {
                    return this.plugIns.ToList();
                }
                else
                {
                    return this.plugIns.Where(p => p.PlugIn.VisibleInList);
                }
            }

            private set
            {
                this.plugIns = value;
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether hidden plug-in are shown.
        /// </summary>
        /// <value>
        ///   <c>true</c> if hidden plug-in are displayed; otherwise, <c>false</c>.
        /// </value>
        [Persistent(DefaultValue = false, EnableVisualConfiguration = false)]
        public bool ShowHiddenPlugIns { get; set; }

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
                    items: new MenuEntryObservableCollection
                    {
                        new MenuSeparator(
                            priority: -31),
                        new MenuEntry(
                            header: "Settings",                            
                            priority: -32,
                            command: this.ShowCommand,
                            icon: "pack://application:,,,/GeoGen.Studio.PlugIns.Settings;component/Images/Icons/settings.png")
                    }));
        }

        /// <summary>
        /// Opens the window.
        /// </summary>
        public void ShowWindow()
        {            
            var list = new List<SettingsPlugInViewModel>();

            foreach (PlugIn plugIn in Loader.PlugIns)
            {               
                if (plugIn.Instances.Any())
                {
                    MainConfig.SaveConfiguration(plugIn.Instances.Last());
                }                

                list.Add(new SettingsPlugInViewModel(plugIn));
            }

            this.PlugIns = list.OrderBy(p => p.PlugIn.Name);

            this.ShowDialog();
        }

        /// <summary>
        /// Writes changes done to the configurations within the dialog back to the actual plug-ins.
        /// </summary>
        private void ApplyChanges()
        {
            foreach (SettingsPlugInViewModel plugInInfo in this.PlugIns)
            {
                // Update enabled status
                plugInInfo.PlugIn.IsEnabled = plugInInfo.IsEnabled;

                // Do not apply configuration changes for non-configurable plug-ins              
                foreach (SettingsPropertyViewModel property in plugInInfo.Properties)
                {
                    MainConfig.SavePropertyValue(property.Property, property.Value);

                    foreach (object instance in plugInInfo.PlugIn.Instances)
                    {
                        property.Property.SetValue(instance, property.Value, null);
                    }                    
                }
            }
        }
    }
}
