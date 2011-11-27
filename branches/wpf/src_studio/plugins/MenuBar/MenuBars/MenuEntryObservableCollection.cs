namespace GeoGen.Studio.PlugIns.MenuBars
{
	using GeoGen.Studio.Utilities.Collections;

	public class MenuEntryObservableCollection : SortedObservableCollection<MenuEntry>
	{
		public MenuEntryObservableCollection() : base(new PriorityComparer()) { }
	}
}
