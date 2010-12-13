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
    public partial class ToolBar : GeoGen.Studio.Utilities.PlugInBase.Control, IPriority
    {
        public double Priority
        {
            get
            {
                return 0;
            }
        }

        private static readonly DependencyProperty ToolBarsProperty = DependencyProperty.Register(
            "ToolBars", typeof(ToolBarEntryObservableCollection), typeof(ToolBar), new PropertyMetadata(new ToolBarEntryObservableCollection()));

        private ToolBarEntryObservableCollection ToolBars
        {
            get
            {
                return (ToolBarEntryObservableCollection)GetValue(ToolBarsProperty);
            }
            set
            {
                SetValue(ToolBarsProperty, value);
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="ToolBar"/> class.
        /// </summary>
        public ToolBar()
        {
            InitializeComponent();

            this.ToolBars.Add(new ToolBarButton(
                toolTip: "Test",
                icon: "C:\\Users\\Matej\\Pictures\\hello-kitty-1.jpg"
            ));

            this.ToolBars.Add(new ToolBarSeparator());

            this.ToolBars.Add(new ToolBarCheckableButton(
                icon: "C:\\Users\\Matej\\Pictures\\hello-kitty-1.jpg",
                isChecked: true
            ));
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
        public void AddToolBar(ToolBarEntry ToolBar)
        {
            // Null IS allowed (for simplicity's sake)
            if (ToolBar == null) return;

            this.ToolBars.Add(ToolBar);
        }

        /// <summary>
        /// Adds one <see cref="Button"/> to the tool bar.
        /// </summary>
        /// <param name="entry">The item.</param>
        public void AddToolBar(Button entry)
        {
            this.AddToolBar((ToolBarEntry)entry);
        }

        /// <summary>
        /// Adds one <see cref="Separator"/> to the tool bar.
        /// </summary>
        /// <param name="entry">The item.</param>
        public void AddToolBar(Separator entry)
        {
            this.AddToolBar((ToolBarEntry)entry);
        }

        /// <summary>
        /// Adds one <see cref="ToggleButton"/> to the tool bar.
        /// </summary>
        /// <param name="entry">The item.</param>
        public void AddToolBar(ToggleButton entry)
        {
            this.AddToolBar((ToolBarEntry)entry);
        }
    }
}
