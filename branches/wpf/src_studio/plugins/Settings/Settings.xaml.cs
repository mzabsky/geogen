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

    public sealed partial class Settings : GeoGen.Studio.Utilities.PlugInBase.WindowBase, INotifyPropertyChanged
    {
        public ICommand ShowCommand { get; private set; }

        public ICommand ApplyCommand { get; private set; }

        public ICommand CloseCommand { get; private set; }

        public ICommand OkCommand { get; private set; }

        public ICommand TogglePlugInCommand { get; private set; }

        public IEnumerable<PlugInInfo> PlugIns { get; private set; }

        [Persistent(DefaultValue = false, EnableVisualConfiguration = false)]
        public bool ShowHiddenPlugIns { get; set; }

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
                    ((PlugInInfo)param).IsEnabled = !((PlugInInfo)param).IsEnabled;
                });

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

        private void ShowWindow()
        {            
            var list = new List<PlugInInfo>();

            foreach (PlugIn plugIn in Loader.PlugIns)
            {               
                MainConfig.SaveConfiguration(plugIn.Instances.Last());                

                list.Add(new PlugInInfo(plugIn));
            }

            this.PlugIns = list.OrderBy(p => p.PlugIn.Name);            

            this.ShowDialog();
        }

        private void ApplyChanges()
        {
            foreach (PlugInInfo plugInInfo in this.PlugIns)
            {
                // Update enabled status
                plugInInfo.PlugIn.IsEnabled = plugInInfo.IsEnabled;

                // Do not apply configuration changes for non-configurable plug-ins              
                foreach (ConfigurablePropertyInfo property in plugInInfo.Properties)
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
