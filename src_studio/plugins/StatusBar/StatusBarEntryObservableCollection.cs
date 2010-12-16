using GeoGen.Studio.Utilities.Collections;

namespace GeoGen.Studio.PlugIns
{
    public class StatusBarEntryObservableCollection : SortedObservableCollection<StatusBarEntry>
    {
        public StatusBarEntryObservableCollection() : base(new PriorityComparer()) { }
    }
}
