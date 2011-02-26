using System.Collections.Generic;
using System.Linq;
using System.Windows.Input;
using System.ComponentModel;
using GeoGen.Studio.Utilities;
using GeoGen.Studio.PlugInLoader;
using GeoGen.Studio.Utilities.Configurability;

namespace GeoGen.Studio.PlugIns
{
    /// <summary>
    /// Interaction logic for UserControl1.xaml
    /// </summary>
    public sealed partial class Settings : GeoGen.Studio.Utilities.PlugInBase.Window, INotifyPropertyChanged
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

        /// <summary>
        /// Initializes a new instance of the <see cref="Settings"/> class.
        /// </summary>
        public Settings()
        {
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
                }
            );

            this.Closing += delegate(object sender, CancelEventArgs args)
            {
                args.Cancel = true;
                this.Hide();
            };

            InitializeComponent();
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
                            priority: -2000
                        ),
                        new MenuEntry(
                            header: "Settings",                            
                            priority: double.MinValue,
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

            foreach(PlugIn plugIn in Loader.PlugIns)
            {
                if (typeof(IConfigurable).IsAssignableFrom(plugIn.Type) && plugIn.IsRunning)
                {
                    MainConfig.SaveConfiguration(plugIn.Instances.Last() as IConfigurable);
                }

                list.Add(new PlugInInfo(plugIn));
            }

            this.PlugIns = list;

            this.ShowDialog();
        }

        private void ApplyChanges()
        {
            foreach(PlugInInfo plugInInfo in this.PlugIns)
            {
                // Update enabled status
                plugInInfo.PlugIn.IsEnabled = plugInInfo.IsEnabled;

                // Do not apply configuration changes for non-configurable plug-ins
                if(!typeof (IConfigurable).IsAssignableFrom(plugInInfo.PlugIn.Type))
                {
                    continue;
                }

                foreach(ConfigurablePropertyInfo property in plugInInfo.Properties)
                {
                    MainConfig.SavePropertyValue(property.Property, property.Value);

                    foreach(IConfigurable instance in plugInInfo.PlugIn.Instances)
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
