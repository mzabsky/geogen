using System;
using System.Windows;
using System.Windows.Controls;
using System.Collections.ObjectModel;

namespace GeoGen.Studio.PlugIns
{
	/// <summary>
	/// Interaction logic for UserControl1.xaml
	/// </summary>
	public partial class ParamGrid : GeoGen.Studio.Utilities.PlugInBase.ControlBase
	{
		private static readonly DependencyPropertyKey ItemsPropertyKey = DependencyProperty.RegisterReadOnly(
			"Items", typeof(ObservableCollection<ScriptArg>), typeof(ParamGrid), new PropertyMetadata(new ObservableCollection<ScriptArg>()));

		public static readonly DependencyProperty ItemsProperty = ItemsPropertyKey.DependencyProperty;

		public ObservableCollection<ScriptArg> Items
		{
			get
			{
				return (ObservableCollection<ScriptArg>)GetValue(ItemsProperty);
			}
			private set
			{
				SetValue(ItemsPropertyKey, value);
			}
		}

		private static readonly DependencyPropertyKey IsEmptyPropertyKey = DependencyProperty.RegisterReadOnly(
			"IsEmpty", typeof(bool), typeof(ParamGrid), new PropertyMetadata(true));

		public static readonly DependencyProperty IsEmptyProperty = IsEmptyPropertyKey.DependencyProperty;

		public bool IsEmpty
		{
			get
			{
				return (bool)GetValue(IsEmptyProperty);
			}
			private set
			{
				SetValue(IsEmptyPropertyKey, value);
			}
		}

		public ParamGrid()
		{
			this.Items = new ObservableCollection<ScriptArg>();
			this.Items.CollectionChanged += this.CollectionChanged;

			InitializeComponent();
		}

		public void Register(IDockManager dockManager)
		{
			dockManager.AddAsDockableContent(this, "Script Parameters", DockingLocation.RightTop);
		}

		public void Register(IGenerator generator){
			this.Items = generator.Args;
			this.Items.CollectionChanged += this.CollectionChanged;
		}

		private void CollectionChanged(object sender, EventArgs args)
		{
			this.IsEmpty = (this.Items.Count == 0);
		}

		private void SetToDefault(object sender, RoutedEventArgs e)
		{
			((ScriptArg)((MenuItem)sender).DataContext).Value = ((ScriptArg)((MenuItem)sender).DataContext).Default;
		}

		private void SetToMaximum(object sender, RoutedEventArgs e)
		{
			((ScriptArg)((MenuItem)sender).DataContext).Value = ((ScriptArg)((MenuItem)sender).DataContext).Maximum;
		}

		private void SetToMinimum(object sender, RoutedEventArgs e)
		{
			((ScriptArg)((MenuItem)sender).DataContext).Value = ((ScriptArg)((MenuItem)sender).DataContext).Minimum;
		}

		private void Randomize(object sender, RoutedEventArgs e)
		{
			Random random = new Random((int)DateTime.Now.Ticks);

			((ScriptArg)((MenuItem)sender).DataContext).Value = (uint)random.Next((int)((ScriptArg)((MenuItem)sender).DataContext).Minimum, (int)((ScriptArg)((MenuItem)sender).DataContext).Maximum + 1);
		}

		private void SetAllToDefault(object sender, RoutedEventArgs e)
		{
			foreach (ScriptArg arg in this.Items)
			{
				arg.Value = arg.Default;
			}
		}

		private void RandomizeAll(object sender, RoutedEventArgs e)
		{
			foreach (ScriptArg arg in this.Items)
			{
				Random random = new Random((int)DateTime.Now.Ticks);

				arg.Value = (uint)random.Next((int)arg.Minimum, (int)arg.Maximum + 1);
			}
		}
	}
}
