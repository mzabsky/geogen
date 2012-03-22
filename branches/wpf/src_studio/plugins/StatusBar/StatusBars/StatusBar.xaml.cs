namespace GeoGen.Studio.PlugIns.StatusBars
{
    using System;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using System.ComponentModel;
    using System.Windows;
    using System.Windows.Data;
    using GeoGen.Studio.PlugIns.Interfaces;
    using GeoGen.Studio.Utilities.Collections;
    using GeoGen.Studio.Utilities.Context;
    using GeoGen.Studio.Utilities.PlugInBase;    

    /// <summary>
    /// Interaction logic for UserControl1.xaml
    /// </summary>
    public partial class StatusBar : ControlBase, IApplicationStatusDisplay, IStatusBar, IProgressDisplay, IPriority, INotifyPropertyChanged
    {
        protected List<Context> applicationStatusContexts = new List<Context>();

        public double Priority
        {
            get
            {
                return 0;
            }
        }

        private static readonly DependencyProperty CurrentApplicationStatusProperty = DependencyProperty.Register(
            "CurrentApplicationStatus", typeof(string), typeof(StatusBar), new PropertyMetadata("Ready"));

        private string CurrentApplicationStatus
        {
            get
            {
                return (string)GetValue(CurrentApplicationStatusProperty);
            }
            set
            {
                SetValue(CurrentApplicationStatusProperty, value);
            }
        }

        private static readonly DependencyProperty ItemsProperty = DependencyProperty.Register(
            "Items", typeof(StatusBarEntryObservableCollection), typeof(StatusBar), new PropertyMetadata(new StatusBarEntryObservableCollection()));

        private StatusBarEntryObservableCollection Items
        {
            get
            {
                return (StatusBarEntryObservableCollection)GetValue(ItemsProperty);
            }
            set
            {
                SetValue(ItemsProperty, value);
            }
        }

        public double CurrentProgress {get; set;}

        public bool IsProgressEnabled {get; set;}

        public StatusBar()
        {

            InitializeComponent();

            ContextManager.ContextChanged += delegate(object o, EventArgs args)
            {
                // Update application status field
                Context applicationStatusContext = ContextManager.GetTopMostKnownActiveContext(this.applicationStatusContexts);

                if (applicationStatusContext == null)
                {
                    this.CurrentApplicationStatus = "Ready";
                }
                else
                {
                    this.CurrentApplicationStatus = applicationStatusContext.Label;
                }

                // Refresh items in the main area of the status bar
                foreach (var item in this.Items)
                {
                    bool isContextActive = ContextManager.IsContextActive(item.Context);
                    if (isContextActive && item.Visibility != Visibility.Visible)
                    {
                        item.Visibility = Visibility.Visible;
                    }
                    else if(!isContextActive && item.Visibility != Visibility.Collapsed){
                        item.Visibility = Visibility.Collapsed;
                    }
                }
            };
        }

        public void Register(IMainWindow mainWindow)
        {
            mainWindow.BottomBars.Add(this);
        }

        public void RegisterApplicationStatusContext(Context context)
        {
            if(!this.applicationStatusContexts.Contains(context))
            {
                this.applicationStatusContexts.Add(context);
            }
        }

        public void AddItem(StatusBarEntry item)
        {
            if (!ContextManager.IsContextActive(item.Context))
            {
                item.Visibility = Visibility.Hidden;
            }
            
            this.Items.Add(item);
        }

        public void SetProgress(object key, double? progress)
        {
            this.IsProgressEnabled = progress.HasValue;

            if (progress.HasValue) {
                this.CurrentProgress = progress.Value;
            }
        }
    }
}
