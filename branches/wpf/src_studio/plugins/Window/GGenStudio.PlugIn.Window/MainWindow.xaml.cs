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

namespace GeoGen.Studio.PlugIns
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : GeoGen.Studio.Utilities.PlugInBase.Window, IPlugIn, IMainWindow
    {
        public static readonly DependencyProperty TopBarsProperty = DependencyProperty.Register(
            "TopBars", typeof(ObservableCollection<object>), typeof(MainWindow), new FrameworkPropertyMetadata(new ObservableCollection<object>())
        );

        public ObservableCollection<object> TopBars
        {
            get
            {
                return (ObservableCollection<object>)GetValue(TopBarsProperty);
            }
            set
            {
                SetValue(TopBarsProperty, value);
            }
        }

        public static readonly DependencyProperty BottomBarsProperty = DependencyProperty.Register(
            "BottomBars", typeof(ObservableCollection<object>), typeof(MainWindow), new FrameworkPropertyMetadata(new ObservableCollection<object>())
        );

        public ObservableCollection<object> BottomBars
        {
            get
            {
                return (ObservableCollection<object>)GetValue(BottomBarsProperty);
            }
            set
            {
                SetValue(BottomBarsProperty, value);
            }
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
