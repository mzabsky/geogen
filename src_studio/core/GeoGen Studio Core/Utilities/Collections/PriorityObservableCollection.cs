using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GeoGen.Studio.Utilities.Collections
{ 
    public class PriorityObservableCollection: SortedObservableCollection<IPriority>
    {
        public PriorityObservableCollection() : base(new PriorityComparer()) { }
    }
}
