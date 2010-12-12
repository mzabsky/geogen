using GeoGen.Studio.Utilities.Collections;

namespace GeoGen.Studio.PlugIns
{
    public class MenuEntryObservableCollection : SortedObservableCollection<MenuEntry>
    {
        public MenuEntryObservableCollection() : base(new PriorityComparer()) { }
    }
}
