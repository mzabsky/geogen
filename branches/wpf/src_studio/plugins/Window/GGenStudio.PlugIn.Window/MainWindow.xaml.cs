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
using System.Collections;
using System.Collections.ObjectModel;
using GeoGen.Studio.PlugInLoader;
using GeoGen.Studio.Utilities;
using GeoGen.Studio.Utilities.Collections;

namespace GeoGen.Studio.PlugIns
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : GeoGen.Studio.Utilities.PlugInBase.Window, IPlugIn, IMainWindow
    {
        public static readonly DependencyProperty TopBarsProperty = DependencyProperty.Register(
            "TopBars", typeof(PriorityObservableCollection), typeof(MainWindow), new FrameworkPropertyMetadata(new PriorityObservableCollection())
        );

        public PriorityObservableCollection TopBars
        {
            get
            {
                return (PriorityObservableCollection)GetValue(TopBarsProperty);
            }
            set
            {
                SetValue(TopBarsProperty, value);
            }
        }

        public static readonly DependencyProperty BottomBarsProperty = DependencyProperty.Register(
            "BottomBars", typeof(PriorityObservableCollection), typeof(MainWindow), new FrameworkPropertyMetadata(new PriorityObservableCollection())
        );

        public PriorityObservableCollection BottomBars
        {
            get
            {
                return (PriorityObservableCollection)GetValue(BottomBarsProperty);
            }
            set
            {
                SetValue(BottomBarsProperty, value);
            }
        }

        public void RegisterInputGesture(InputGesture gesture, ICommand command)
        {
            InputBinding inputBinding = new InputBinding(command, gesture);
            this.InputBindings.Add(inputBinding);
        }

        public MainWindow()
        {
            InitializeComponent();
        }

        public void Register(){
            this.Show();
        }        
    }
}
