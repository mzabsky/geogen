using GeoGen.Studio.Utilities.Collections;

namespace GeoGen.Studio.PlugIns
{
    public class ToolBarEntryObservableCollection : SortedObservableCollection<ToolBarEntry>
    {
        public ToolBarEntryObservableCollection() : base(new PriorityComparer()) { }
    }
}
