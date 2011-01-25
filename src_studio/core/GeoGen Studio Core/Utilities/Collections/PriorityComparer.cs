using System.Collections.Generic;

namespace GeoGen.Studio.Utilities.Collections
{
    /// <summary>
    /// Compares two objects by their <see cref="IPriority">priority</see>.
    /// </summary>
    public class PriorityComparer : IComparer<IPriority>
    {
        int IComparer<IPriority>.Compare(IPriority a, IPriority b)
        {
            if (a.Priority > b.Priority)
                return -1;
            if (a.Priority < b.Priority)
                return 1;

            return 0;
        }
    }
}
