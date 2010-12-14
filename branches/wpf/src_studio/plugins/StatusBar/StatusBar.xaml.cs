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
using GeoGen.Studio.Utilities.Collections;
using GeoGen.Studio.Utilities.Context;

namespace GeoGen.Studio.PlugIns
{
    /// <summary>
    /// Interaction logic for UserControl1.xaml
    /// </summary>
    public partial class StatusBar : GeoGen.Studio.Utilities.PlugInBase.Control, IApplicationStatusDisplay, IPriority
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

        public StatusBar()
        {
            InitializeComponent();

            ContextManager.ContextChanged += delegate(object o, EventArgs args)
            {
                Context applicationStatusContext = ContextManager.GetTopMostKnownActiveContext(this.applicationStatusContexts);

                if (applicationStatusContext == null)
                {
                    this.CurrentApplicationStatus = "Ready";
                }
                else
                {
                    this.CurrentApplicationStatus = applicationStatusContext.Label;
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
    }
}
