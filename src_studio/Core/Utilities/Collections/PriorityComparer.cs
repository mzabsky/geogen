namespace GeoGen.Studio.Utilities.Collections
{
    using System.Collections.Generic;

    /// <summary>
    /// Compares two objects by their <see cref="IPriority">priority</see> in descending order.
    /// </summary>
    public class PriorityComparer : IComparer<IPriority>
    {
        public int Compare(IPriority a, IPriority b)
        {
            if (a.Priority > b.Priority)
                return -1;
            if (a.Priority < b.Priority)
                return 1;

            return 0;
        }
    }
}
