using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GeoGen.Studio.Utilities.Collections
{
    /// <summary>
    /// Represents a dynamic data collection that provides notifications when items get added, removed, or when the whole list is refreshed. The items are sorted by their <see cref="IPriority">priority</see>.
    /// </summary>
    public class PriorityObservableCollection: SortedObservableCollection<IPriority>
    {
        /// <summary>
        /// Initializes a new empty instance of the <see cref="PriorityObservableCollection"/> class.
        /// </summary>
        public PriorityObservableCollection() : base(new PriorityComparer()) { }
    }
}
