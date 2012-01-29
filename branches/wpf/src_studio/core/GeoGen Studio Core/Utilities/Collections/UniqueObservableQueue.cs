using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;

namespace GeoGen.Studio.Utilities.Collections
{
    /// <summary>
    /// Collection of unique records, provides update notifications. Has limited capacity - items from the back ot the collection will
    /// be discarded when the capacity is reached.
    /// </summary>
    /// <typeparam name="TValue">The type of the value.</typeparam>
    public class UniqueObservableQueue<TValue> : ICollection<TValue>, INotifyCollectionChanged
    {
        private readonly List<TValue> values = new List<TValue>();

        /// <summary>
        /// Occurs when the collection changes.
        /// </summary>
        public event NotifyCollectionChangedEventHandler CollectionChanged;

        /// <summary>
        /// Capacity of the collection.
        /// </summary>
        public int Capacity { get; private set; }

        /// <summary>
        /// Comparer used to detect duplicates in the collection.
        /// </summary>
        public IEqualityComparer<TValue> Comparer { get; private set; }

        public int Count
        {
            get
            {
                return this.values.Count;
            }
        }

        public bool IsReadOnly
        {
            get
            {
                return false;
            }
        }

        /// <summary>
        /// Gets the value at the specified index.
        /// </summary>
        public TValue this [int index]
        {
            get
            {
                return this.values[index];
            }
        }

        /// <summary>
        /// Default capacity off the collection.
        /// </summary>
        /// <value>The default capacity.</value>
        public static int DefaultCapacity
        {
            get
            {
                return 20;
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="UniqueObservableQueue&lt;TValue&gt;"/> class with default capacity (20).
        /// </summary>
        public UniqueObservableQueue()
        {
            this.Capacity = UniqueObservableQueue<TValue>.DefaultCapacity;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="UniqueObservableQueue&lt;TValue&gt;"/> class.
        /// </summary>
        /// <param name="capacity">The capacity.</param>
        /// <param name="comparer">The comparer.</param>
        public UniqueObservableQueue(int capacity = 20, IEqualityComparer<TValue> comparer = null)
        {
            this.values.Capacity = capacity;
            this.Capacity = capacity;
            this.Comparer = comparer;
        }

        /// <summary>
        /// Adds new item to the collection. If that item already is in the collection, it will be moved to the first position. If the collection is full, the last item will be dropped.
        /// </summary>
        /// <param name="item">The item.</param>
        public virtual void Add(TValue item)
        {
            // The collection already contains the item - we will be only moving.            
            if(values.Contains(item, this.Comparer))
            {
                int oldIndex = -1;
                TValue oldItem = default(TValue);

                // Find the item in the array (cannot use IndexOf, because it doesn't support custom comparer).
                foreach (TValue value in this.values.Where(value => this.Comparer.Equals(item, value)))
                {
                    oldIndex = values.IndexOf(value);
                    oldItem = value;
                    break;
                }

                // The item is already the first -> no need to do anything.
                if (oldIndex == 0) return;

                values.RemoveAt(oldIndex);
                values.Insert(0, item);

                this.OnCollectionChanged(
                    new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Remove, oldItem, oldIndex)
                    );

                this.OnCollectionChanged(
                    new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Add, item, 0)
                    );

                return;
            }

            // Crop the collection if it would be too long after the addition.
            if(this.values.Count > this.Capacity - 1)
            {
                TValue oldItem = this.values[this.Capacity - 1];

                this.values.RemoveAt(this.Capacity - 1);

                this.OnCollectionChanged(
                    new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Remove, oldItem)
                    );
            }

            // Finally add the item.
            this.values.Insert(0, item);
            this.OnCollectionChanged(
                new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Add, item, 0)
                );
        }

        public void CopyTo(TValue[] array, int arrayIndex)
        {
            this.values.CopyTo(array, arrayIndex);
        }

        public virtual bool Remove(TValue item)
        {
            bool result = this.values.Remove(item);
            this.OnCollectionChanged(
                new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Remove, item)
                );

            return result;
        }

        public virtual void Clear()
        {
            this.values.Clear();
            this.OnCollectionChanged(
                new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset)
                );
        }

        public bool Contains(TValue item)
        {
            return this.values.Contains(item);
        }

        public virtual IEnumerator<TValue> GetEnumerator()
        {
            return this.values.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        protected virtual void OnCollectionChanged(NotifyCollectionChangedEventArgs e)
        {
            if (this.CollectionChanged != null)
            {
                this.CollectionChanged(this, e);
            }
        }
    }
}
