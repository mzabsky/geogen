using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GeoGen.Studio.Utilities.Collections
{
    public class PriorityComparer : IComparer<IPriority>
    {
        int IComparer<IPriority>.Compare(IPriority a, IPriority b)
        {
            if (a.Priority > b.Priority)
                return -1;
            if (a.Priority < b.Priority)
                return 1;
            else
                return 0;
        }
    }
}
