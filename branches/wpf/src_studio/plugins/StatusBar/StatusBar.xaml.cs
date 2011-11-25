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
using System.Collections.ObjectModel;

namespace GeoGen.Studio.PlugIns
{
	/// <summary>
	/// Interaction logic for UserControl1.xaml
	/// </summary>
	public partial class StatusBar : GeoGen.Studio.Utilities.PlugInBase.ControlBase, IApplicationStatusDisplay, IStatusBar, IPriority
	{
		protected List<Context> applicationStatusContexts = new List<Context>();
		protected List<Context> itemContexts = new List<Context>();

		public double Priority
		{
			get
			{
				return 0;
			}
		}
		
		//public ObservableCollection<Visibility> ItemVisibilities {get; set;}

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

		private static readonly DependencyProperty ItemVisibilitiesProperty = DependencyProperty.Register(
			"ItemVisibilities", typeof(ObservableCollection<Visibility>), typeof(StatusBar), new PropertyMetadata(new ObservableCollection<Visibility>()));

		public ObservableCollection<Visibility> ItemVisibilities
		{
			get
			{
				return (ObservableCollection<Visibility>)GetValue(ItemVisibilitiesProperty);
			}
			set
			{
				SetValue(ItemVisibilitiesProperty, value);
			}
		}

		public StatusBar()
		{
			//this.ItemVisibilities = new ObservableCollection<Visibility>();

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

				// Update item visibilities
				this.ItemVisibilities.Clear();

				foreach (Context context in this.itemContexts)
				{
					if(ContextManager.IsContextActive(context))
					{
						this.ItemVisibilities.Add(Visibility.Visible);
					}
					else
					{
						this.ItemVisibilities.Add(Visibility.Collapsed);
					}
				}
			};

			/*this.Items.Add(
				new StatusBarEntry
				{
					ValueBinding = new Binding
					{
						Path = new PropertyPath("CurrentApplicationStatus"),
						Source = this,
					}
				}
			);*/
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
			this.itemContexts.Add(item.Context);
			
			this.ItemVisibilities.Add(ContextManager.IsContextActive(item.Context) ? Visibility.Visible : Visibility.Collapsed);

			BindingOperations.SetBinding(
				item,
				StatusBarEntry.VisibilityProperty,
				new Binding{
					Path = new PropertyPath("ItemVisibilities[" + (this.ItemVisibilities.Count - 1) + "]"),					
					RelativeSource = new RelativeSource(RelativeSourceMode.FindAncestor, typeof(IStatusBar), 1)
				}
			);

			this.Items.Add(item);
		}
	}
}
