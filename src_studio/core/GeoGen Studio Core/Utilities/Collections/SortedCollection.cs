// http://softcollections.codeplex.com/
namespace GeoGen.Studio.Utilities.Collections
{
    using System;
    using System.Collections;
    using System.Collections.Generic;
    using System.Linq;

    /// <summary>
    /// Represents a dynamic data collection. The items are sorted by a given <see cref="IComparer"/>.
    /// </summary>
    public class SortedCollection<TValue> : IList<TValue>, IList
    {
        /// <summary>
        /// Default value for Capacity property.
        /// </summary>
        private const int DefaultCapacity = 4;

        /// <summary>
        /// Array used when there are no items in the collection. Shared by all instances.
        /// </summary>
        private static readonly TValue[] EmptyBackingArray;

        /// <summary>
        /// Actual collection holding the backingArray of this collection.
        /// </summary>
        private TValue[] backingArray;

        /// <summary>
        /// Initializes a new instance of the <see cref="SortedCollection&lt;TValue&gt;"/> class.
        /// </summary>
        /// <param name="comparer">The comparer.</param>
        public SortedCollection(IComparer<TValue> comparer)
        {
            this.backingArray = EmptyBackingArray;
            this.Comparer = comparer;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="SortedCollection&lt;TValue&gt;"/> class.
        /// </summary>
        public SortedCollection()
        {
            this.backingArray = EmptyBackingArray;
            this.Comparer = Comparer<TValue>.Default;
        }

        /// <summary>
        /// Initializes the <see cref="SortedCollection&lt;TValue&gt;"/> class.
        /// </summary>
        static SortedCollection()
        {
            EmptyBackingArray = new TValue[0];
        }

        /// <summary>
        /// Capacity of the collection.
        /// </summary>
        /// <value>The capacity.</value>
        public int Capacity
        {
            get { return this.backingArray.Length; }
            set
            {
                if (this.backingArray.Length == value)
                {
                    return;
                }

                if (value < this.Count)
                {
                    throw new ArgumentException("Too small capacity.");
                }

                if (value > 0)
                {
                    var tempValues = new TValue[value];
                    if (this.Count > 0)
                    {
                        // copy only when size is greater than zero
                        Array.Copy(this.backingArray, 0, tempValues, 0, this.Count);
                    }
                    this.backingArray = tempValues;
                }
                else
                {
                    this.backingArray = EmptyBackingArray;
                }
            }
        }

        public IComparer<TValue> Comparer { get; protected set; }

        public int Count { get; private set; }

        public virtual TValue this[int index]
        {
            get
            {
                if (index < 0 || index >= this.Count)
                {
                    throw new ArgumentOutOfRangeException();
                }

                return this.backingArray[index];
            }

            set
            {
                if (index < 0 || index >= this.Count)
                {
                    throw new ArgumentOutOfRangeException();
                }

                this.UnsubscribeItem(this.backingArray[index]);
                this.SubscribeItem(value);
                this.backingArray[index] = value;
            }
        }

        bool ICollection.IsSynchronized
        {
            get { return false; }
        }

        object ICollection.SyncRoot
        {
            get { return new object(); }
        }

        bool ICollection<TValue>.IsReadOnly
        {
            get { return false; }
        }

        bool IList.IsFixedSize
        {
            get { return true; }
        }

        bool IList.IsReadOnly
        {
            get { return false; }
        }

        object IList.this[int index]
        {
            get
            {
                return this[index];
            }

            set
            {
                this[index] = (TValue)value;
            }
        }

        public void Add(TValue value)
        {
            if (value == null)
            {
                throw new ArgumentNullException("value");
            }
            // check where the element should be placed
            int index = Array.BinarySearch(this.backingArray, 0, this.Count, value, this.Comparer);
            if (index < 0)
            {
                // xor
                index = ~index;
            }

            while (index < this.Count && this.Comparer.Compare(this.backingArray[index], value) == 0)
            {
                index++;
            }

            Insert(index, value);
        }

        public virtual void Clear()
        {
            foreach (var value in this.backingArray)
            {
                this.UnsubscribeItem(value);
            }

            Array.Clear(this.backingArray, 0, this.Count);
            this.Count = 0;
        }

        public bool Contains(TValue value)
        {
            return this.IndexOf(value) >= 0;
        }

        public void CopyTo(TValue[] array, int arrayIndex)
        {
            Array.Copy(this.backingArray, 0, array, arrayIndex, this.Count);
        }

        public IEnumerator<TValue> GetEnumerator()
        {
            // Skip the 0s at the end of the pre-allocated array.
            return this.backingArray.Take(this.Count).GetEnumerator();
        }

        public int IndexOf(TValue value)
        {
            if (value == null)
            {
                throw new ArgumentNullException("value");
            }
            int index = Array.BinarySearch(this.backingArray, 0, this.Count, value, this.Comparer);
            if (index >= 0)
            {
                return index;
            }
            return -1;
        }

        public virtual void Insert(int index, TValue value)
        {
            if (value == null)
            {
                throw new ArgumentNullException("value");
            }
            if (index < 0 || index > this.Count)
            {
                throw new ArgumentOutOfRangeException();
            }
            if (this.Count == this.backingArray.Length)
            {
                this.ExpandBackingArray();
            }
            if (index < this.Count)
            {
                Array.Copy(this.backingArray, index, this.backingArray, index + 1, this.Count - index);
            }
            this.SubscribeItem(value);
            this.backingArray[index] = value;
            this.Count++;
        }

        public bool Remove(TValue value)
        {
            int index = this.IndexOf(value);
            if (index < 0)
            {
                return false;
            }
            RemoveAt(index);
            return true;
        }

        public virtual void RemoveAt(int index)
        {
            if (index < 0 || index >= this.Count)
            {
                throw new ArgumentOutOfRangeException();
            }

            this.UnsubscribeItem(this[index]);
            this.Count--;
            Array.Copy(this.backingArray, index + 1, this.backingArray, index, this.Count - index);
            this.backingArray[this.Count] = default(TValue);
        }

        /// <summary>
        /// Called when a new item is added to the collection - allows subclasses to subscribe to events on the item.
        /// </summary>
        /// <param name="item">The item.</param>
        protected virtual void SubscribeItem(TValue item)
        {
        }

        /// <summary>
        /// Called when a new item is removed from the collection - allows subclasses to unsubscribe from events on the item.
        /// </summary>
        /// <param name="item">The item.</param>
        protected virtual void UnsubscribeItem(TValue item)
        {
        }

        /// <summary>
        /// Expands capacity of the backing collection.
        /// </summary>
        private void ExpandBackingArray()
        {
            // double the capacity
            int num = this.backingArray.Length == 0 ? DefaultCapacity : this.backingArray.Length * 2;

            this.Capacity = num;
        }

        void ICollection.CopyTo(System.Array array, int arrayIndex)
        {
            Array.Copy(this.backingArray, 0, array, arrayIndex, this.Count);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return this.GetEnumerator();
        }

        int IList.Add(object value)
        {
            this.Add((TValue)value);
            return this.IndexOf((TValue)value);
        }

        bool IList.Contains(object value)
        {
            return this.Contains((TValue)value);
        }

        int IList.IndexOf(object value)
        {
            return this.IndexOf((TValue)value);
        }

        void IList.Insert(int index, object value)
        {
            this.Insert(index, (TValue)value);
        }

        void IList.Remove(object value)
        {
            this.Remove((TValue)value);
        }
    }
}
