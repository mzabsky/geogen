namespace GeoGen.Studio.PlugIns.ToolBars
{
	using System.ComponentModel;
	using System.Windows;
	using System.Windows.Controls;
	using System.Windows.Controls.Primitives;

	using GeoGen.Studio.Utilities.Collections;

	public abstract class ToolBarEntry : UserControl, IPriority, INotifyPropertyChanged
	{
		#region Constants and Fields

		private static new readonly DependencyProperty DataContextProperty = DependencyProperty.Register(
			"DataContext", typeof(object), typeof(ToolBarEntry), new PropertyMetadata(null));

		private static new readonly DependencyProperty ToolTipProperty = DependencyProperty.Register(
			"ToolTip", typeof(object), typeof(ToolBarEntry), new PropertyMetadata());

		#endregion

		#region Public Events

		public event PropertyChangedEventHandler PropertyChanged;

		#endregion

		#region Public Properties

		public object DataContext
		{
			get
			{
				return this.GetValue(DataContextProperty);
			}

			set
			{
				this.SetValue(DataContextProperty, value);
			}
		}

		public double Priority { get; set; }

		public object ToolTip
		{
			get
			{
				return this.GetValue(ToolTipProperty);
			}

			set
			{
				this.SetValue(ToolTipProperty, value);
			}
		}

		#endregion

		#region Methods

		protected void OnPropertyChanged(string info)
		{
			if (this.PropertyChanged != null)
			{
				this.PropertyChanged(this, new PropertyChangedEventArgs(info));
			}
		}

		#endregion

		#region Operators

		public static implicit operator ToolBarEntry(Button victim)
		{
			ToolBarEntry converted = new ToolBarButton(
				icon: victim.Content, 
				toolTip: victim.ToolTip, 
				command: victim.Command
			);

			return converted;
		}

		public static implicit operator ToolBarEntry(ToggleButton victim)
		{
			ToolBarEntry converted = new ToolBarCheckableButton(
				icon: victim.Content, 
				toolTip: victim.ToolTip,
				command: victim.Command,
				isChecked: (bool)victim.IsChecked);

			return converted;
		}

		public static implicit operator ToolBarEntry(Separator victim)
		{
			ToolBarEntry converted = new ToolBarSeparator();
			return converted;
		}

		#endregion
	}
}
