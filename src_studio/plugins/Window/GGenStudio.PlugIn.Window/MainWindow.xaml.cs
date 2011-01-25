using System.Windows;
using System.Windows.Input;
using GeoGen.Studio.PlugInLoader;
using GeoGen.Studio.Utilities.Collections;

namespace GeoGen.Studio.PlugIns
{
    /// <summary>
    /// Main window for the application, that can place any number of horizontal bars on top and bottom side of the window and one control as its content.
    /// </summary>
    public partial class MainWindow : IMainWindow
    {
        private static readonly DependencyProperty topBarsProperty = DependencyProperty.Register(
            "TopBars", typeof(PriorityObservableCollection), typeof(MainWindow), new FrameworkPropertyMetadata(new PriorityObservableCollection())
        );

        public PriorityObservableCollection TopBars
        {
            get
            {
                return (PriorityObservableCollection)GetValue(topBarsProperty);
            }
            set
            {
                SetValue(topBarsProperty, value);
            }
        }

        private static readonly DependencyProperty bottomBarsProperty = DependencyProperty.Register(
            "BottomBars", typeof(PriorityObservableCollection), typeof(MainWindow), new FrameworkPropertyMetadata(new PriorityObservableCollection())
        );

        public PriorityObservableCollection BottomBars
        {
            get
            {
                return (PriorityObservableCollection)GetValue(bottomBarsProperty);
            }
            set
            {
                SetValue(bottomBarsProperty, value);
            }
        }


        public void RegisterInputGesture(InputGesture gesture, ICommand command)
        {
            InputBinding inputBinding = new InputBinding(command, gesture);
            this.InputBindings.Add(inputBinding);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MainWindow"/> class.
        /// </summary>
        public MainWindow()
        {
            InitializeComponent();

            this.Closed += delegate
            {
                Application.Current.Shutdown();
            };
        }

        /// <summary>
        /// Registers this instance with the <see cref="Loader"/>.
        /// </summary>
        public void Register(){
            this.Show();
        }        
    }
}
