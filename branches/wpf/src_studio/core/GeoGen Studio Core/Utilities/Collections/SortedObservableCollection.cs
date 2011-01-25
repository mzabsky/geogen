// http://softcollections.codeplex.com/

using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;

namespace GeoGen.Studio.Utilities.Collections
{
    /// <summary>
    /// Represents a dynamic data collection that provides notifications when items get added, removed, or when the whole list is refreshed. The items are sorted by a given comparer.
    /// </summary>
    /// <typeparam name="TValue">The type of the value.</typeparam>
    public class SortedObservableCollection<TValue> : SortedCollection<TValue>, INotifyPropertyChanged, INotifyCollectionChanged
    {
        /// <summary>
        /// Initializes a new empty instance of the <see cref="SortedObservableCollection&lt;TValue&gt;"/> class.
        /// </summary>
        public SortedObservableCollection()
        { }

        /// <summary>
        /// Initializes a new empty instance of the <see cref="SortedObservableCollection&lt;TValue&gt;"/> class.
        /// </summary>
        /// <param name="comparer">The comparer.</param>
        public SortedObservableCollection(IComparer<TValue> comparer) : base(comparer) { }

        // Events
        /// <summary>
        /// Occurs when the collection changes.
        /// </summary>
        public event NotifyCollectionChangedEventHandler CollectionChanged;

        /// <summary>
        /// Occurs when a property value changes.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnCollectionChanged(NotifyCollectionChangedEventArgs e)
        {
            if (this.CollectionChanged != null)
            {
                this.CollectionChanged(this, e);
            }
        }

        private void OnCollectionChanged(NotifyCollectionChangedAction action, object item, int index)
        {
            this.OnCollectionChanged(new NotifyCollectionChangedEventArgs(action, item, index));
        }

        private void OnCollectionChanged(NotifyCollectionChangedAction action, object oldItem, object newItem, int index)
        {
            this.OnCollectionChanged(new NotifyCollectionChangedEventArgs(action, newItem, oldItem, index));
        }

/*
        private void OnCollectionChanged(NotifyCollectionChangedAction action, object item, int index, int oldIndex)
        {
            this.OnCollectionChanged(new NotifyCollectionChangedEventArgs(action, item, index, oldIndex));
        }
*/

        private void OnCollectionReset()
        {
            this.OnCollectionChanged(new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        protected virtual void OnPropertyChanged(PropertyChangedEventArgs e)
        {
            if (this.PropertyChanged != null)
            {
                this.PropertyChanged(this, e);
            }
        }

        private void OnPropertyChanged(string propertyName)
        {
            this.OnPropertyChanged(new PropertyChangedEventArgs(propertyName));
        }

        public override void Insert(int index, TValue value)
        {
            base.Insert(index, value);
            this.OnPropertyChanged("Count");
            this.OnPropertyChanged("Item[]");
            this.OnCollectionChanged(NotifyCollectionChangedAction.Add, value, index);
        }

        public override void RemoveAt(int index)
        {
            var item = this[index];
            base.RemoveAt(index);
            this.OnPropertyChanged("Item[]");
            this.OnPropertyChanged("Count");
            this.OnCollectionChanged(NotifyCollectionChangedAction.Remove, item, index);
        }

        public override TValue this[int index]
        {
            get
            {
                return base[index];
            }
            set
            {
                var oldItem = base[index];
                base[index] = value;
                this.OnPropertyChanged("Item[]");
                this.OnCollectionChanged(NotifyCollectionChangedAction.Replace, oldItem, value, index);
            }
        }

        public override void Clear()
        {
            base.Clear();
            OnCollectionReset();
        }
    }
}
