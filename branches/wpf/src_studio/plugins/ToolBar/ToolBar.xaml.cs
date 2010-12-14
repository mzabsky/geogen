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
using System.Windows.Controls.Primitives;

namespace GeoGen.Studio.PlugIns
{
    /// <summary>
    /// A horizontal tool bar which displays items provided by other plug-ins.
    /// </summary>
    public partial class ToolBar : GeoGen.Studio.Utilities.PlugInBase.Control, IToolBar, IPriority
    {
        public double Priority
        {
            get
            {
                return 0;
            }
        }

        private static readonly DependencyProperty ItemsProperty = DependencyProperty.Register(
            "Items", typeof(ToolBarEntryObservableCollection), typeof(ToolBar), new PropertyMetadata(new ToolBarEntryObservableCollection()));

        private ToolBarEntryObservableCollection Items
        {
            get
            {
                return (ToolBarEntryObservableCollection)GetValue(ItemsProperty);
            }
            set
            {
                SetValue(ItemsProperty, value);
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="ToolBar"/> class.
        /// </summary>
        public ToolBar()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Adds the toolbar into the main window.
        /// </summary>
        /// <param name="mainWindow">The main window.</param>
        public void Register(IMainWindow mainWindow)
        {
            mainWindow.TopBars.Add(this);
        }

        /// <summary>
        /// Adds one <see cref="ToolBarEntry"/> to the tool bar.
        /// </summary>
        /// <param name="ToolBar">The item.</param>
        public void AddItem(ToolBarEntry item)
        {
            // Null IS allowed (for simplicity's sake)
            if (item == null) return;

            this.Items.Add(item);
        }

        /// <summary>
        /// Adds one <see cref="Button"/> to the tool bar.
        /// </summary>
        /// <param name="entry">The item.</param>
        public void AddItem(Button entry)
        {
            this.AddItem((ToolBarEntry)entry);
        }

        /// <summary>
        /// Adds one <see cref="Separator"/> to the tool bar.
        /// </summary>
        /// <param name="entry">The item.</param>
        public void AddItem(Separator entry)
        {
            this.AddItem((ToolBarEntry)entry);
        }

        /// <summary>
        /// Adds one <see cref="ToggleButton"/> to the tool bar.
        /// </summary>
        /// <param name="entry">The item.</param>
        public void AddItem(ToggleButton entry)
        {
            this.AddItem((ToolBarEntry)entry);
        }
    }
}
