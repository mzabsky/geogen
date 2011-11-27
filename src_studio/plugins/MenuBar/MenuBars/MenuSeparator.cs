namespace GeoGen.Studio.PlugIns.MenuBars
{
	using GeoGen.Studio.Utilities.Collections;

	public class MenuSeparator: IPriority
	{
		public double Priority { get; protected set; }

		public MenuSeparator(double priority = 0)
		{
			this.Priority = priority;
		}
	}
}
