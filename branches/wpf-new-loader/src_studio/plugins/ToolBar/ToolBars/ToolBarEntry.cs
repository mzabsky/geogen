namespace GeoGen.Studio.PlugIns.ToolBars
{
	using System.ComponentModel;
	using System.Windows.Controls;
	using System.Windows.Controls.Primitives;

	using GeoGen.Studio.Utilities.Collections;

	public abstract class ToolBarEntry : UserControl, IPriority, INotifyPropertyChanged
	{
		public event PropertyChangedEventHandler PropertyChanged;

		public double Priority { get; set; }

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
				command: victim.Command);

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
