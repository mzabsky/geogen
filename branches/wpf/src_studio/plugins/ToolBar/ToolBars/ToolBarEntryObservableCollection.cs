namespace GeoGen.Studio.PlugIns.ToolBars
{
    using GeoGen.Studio.Utilities.Collections;

    /// <summary>
    /// Sorted observable collection of <see cref="ToolBarEntry"/>
    /// </summary>
    public class ToolBarEntryObservableCollection : GroupedPriorityObservableCollection<ToolBarEntry>
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="ToolBarEntryObservableCollection"/> class.
        /// </summary>
        public ToolBarEntryObservableCollection()
            : base(typeof(ToolBarSeparator))
        {
        }
    }
}
