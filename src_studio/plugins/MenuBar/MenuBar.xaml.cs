using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using GeoGen.Studio.Utilities;
using GeoGen.Studio.Utilities.Collections;
using System.Windows.Markup;
using System.Collections;
using System.Collections.ObjectModel;

namespace GeoGen.Studio.PlugIns
{
    /// <summary>
    /// A horizontal menu bar which displays items provided by other plug-ins.
    /// </summary>
    public partial class MenuBar : GeoGen.Studio.Utilities.PlugInBase.Control, IMenuBar, IPriority
    {
        public double Priority
        {
            get
            {
                return 1;
            }
        }

        private static readonly DependencyProperty MenusProperty = DependencyProperty.Register(
            "Menus", typeof(MenuEntryObservableCollection), typeof(MenuBar), new PropertyMetadata(new MenuEntryObservableCollection()));

        private MenuEntryObservableCollection Menus
        {
            get
            {
                return (MenuEntryObservableCollection)GetValue(MenusProperty);
            }
            set
            {
                SetValue(MenusProperty, value);
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MenuBar"/> class.
        /// </summary>
        public MenuBar()
        {
            InitializeComponent();

            // The menu contains only Exit item by default
            MenuEntry fileMenu = new MenuEntry(
                header: "File",
                priority: 0
            );
            
            this.AddMenu(fileMenu);
            
            fileMenu.Items.Add(
                new MenuEntry(
                    header: "Exit",
                    priority: Double.MinValue,
                    command: new RelayCommand(param => App.Current.Shutdown()),
                    inputGestureText: "Alt+F4"
                )
            );
        }

        /// <summary>
        /// Plug-in registrator. Do not call manually.
        /// </summary>
        /// <param name="mainWindow">The main window.</param>
        public void Register(IMainWindow mainWindow)
        {
            mainWindow.TopBars.Add(this);
        }

        /// <summary>
        /// Merges one <see cref="MenuEntry"/> into the menu item hierarchy.
        /// </summary>
        /// <param name="menu">The menu item.</param>
        public void AddMenu(MenuEntry menu)
        {
            // Null IS allowed (for simplicity's sake)
            if (menu == null) return;

            foreach (MenuEntry ownItem in this.Menus)
            {
                try
                {
                    if (ownItem.Header == menu.Header)
                    {
                        ownItem.MergeWith(menu);

                        return;
                    }
                }
                catch(Exception e){

                }
            }

            this.Menus.Add(menu);
        }

        /// <summary>
        /// Merges one <see cref="MenuItem"/> into the menu item hierarchy.
        /// </summary>
        /// <param name="menu">The menu item.</param>
        public void AddMenu(MenuItem menu)
        {          
            this.AddMenu((MenuEntry) menu);
        }
    }
}
