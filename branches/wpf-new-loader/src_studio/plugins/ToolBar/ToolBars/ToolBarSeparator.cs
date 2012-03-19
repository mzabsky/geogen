namespace GeoGen.Studio.PlugIns.ToolBars
{
	using System.Windows;
	using GeoGen.Studio.Utilities.Collections;

	public class ToolBarSeparator: ToolBarEntry
	{
		static ToolBarSeparator()
		{
			DefaultStyleKeyProperty.OverrideMetadata(typeof(ToolBarSeparator),
				new FrameworkPropertyMetadata(typeof(ToolBarSeparator)));            
		}

		public ToolBarSeparator() {}

		public ToolBarSeparator(double priority) 
		{
			this.Priority = priority;
		}
	}
}
