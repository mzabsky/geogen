namespace GeoGen.Studio.Utilities.Collections
{
    using System.Collections.Generic;
    using System.ComponentModel;

    /// <summary>
    /// Represents a dynamic data collection that provides notifications when items get added, removed, or when the whole list is refreshed. The items are sorted by their <see cref="IPriority">priority</see>.
    /// </summary>
    public class PriorityObservableCollection: SortedObservableCollection<IPriority>
    {
        /// <summary>
        /// Initializes a new empty instance of the <see cref="PriorityObservableCollection"/> class.
        /// </summary>
        public PriorityObservableCollection() : base(new PriorityComparer()) { }

        protected override void ItemPropertyChanged(object o, PropertyChangedEventArgs args)
        {
            if (args.PropertyName != "Priority")
            {
                return;
            }
            
            this.Remove((IPriority)o);
            this.Add((IPriority)o);
        }
    }

    /// <summary>
    /// Represents a dynamic data collection that provides notifications when items get added, removed, or when the whole list is refreshed. The items are sorted by their <see cref="IPriority">priority</see>.
    /// </summary>
    public class PriorityObservableCollection<T>: SortedObservableCollection<T> where T:IPriority
    {
        public PriorityObservableCollection()
        {
            this.Comparer = (IComparer<T>) new PriorityComparer();
        }

        protected override void ItemPropertyChanged(object o, PropertyChangedEventArgs args)
        {
            if (args.PropertyName != "Priority")
            {
                return;
            }

            this.Remove((T)o);
            this.Add((T)o);
        }
    }
}
