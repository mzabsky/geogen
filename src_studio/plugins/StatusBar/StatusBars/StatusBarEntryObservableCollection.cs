namespace GeoGen.Studio.PlugIns.StatusBars
{
	using GeoGen.Studio.Utilities.Collections;

	public class StatusBarEntryObservableCollection : SortedObservableCollection<StatusBarEntry>
	{
		public StatusBarEntryObservableCollection() : base(new PriorityComparer()) { }
	}
}
