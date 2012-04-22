namespace GeoGen.Studio.Utilities.Collections
{
    using System;
    using System.Collections;
    using System.Collections.Generic;
    using System.Collections.Specialized;
    using System.ComponentModel;

    /// <summary>
    /// Read-only wrapper around observable collection.
    /// </summary>
    /// <typeparam name="T">
    /// Type of items in the collection.
    /// </typeparam>
    public class ReadOnlyObservableCollectionView<T> : 
        IEnumerable<T>, 
        IEnumerable, 
        ICollection, 
        ICollection<T>, 
        INotifyCollectionChanged, 
        INotifyPropertyChanged
    {
        /// <summary>
        /// The wrapped collection.
        /// </summary>
        private readonly ICollection<T> collection;

        /// <summary>
        /// Initializes a new instance of the <see cref="ReadOnlyObservableCollectionView&lt;T&gt;"/> class.
        /// </summary>
        /// <param name="collection">The collection.</param>
        public ReadOnlyObservableCollectionView(ICollection<T> collection)
        {
            if (!(collection is INotifyCollectionChanged))
            {
                throw new ArgumentException("Collection must implement INotifyCollectonChanged.");
            }

            this.collection = collection;

            ((INotifyCollectionChanged)this.collection).CollectionChanged += delegate(object sender, NotifyCollectionChangedEventArgs args)
                {
                    if (this.CollectionChanged != null)
                    {
                        this.CollectionChanged(sender, args);
                    }
                };

            if (this.collection is INotifyPropertyChanged)
            {
                ((INotifyPropertyChanged)this.collection).PropertyChanged += delegate(object sender, PropertyChangedEventArgs args)
                {
                    if (this.PropertyChanged != null)
                    {
                        this.PropertyChanged(sender, args);
                    }
                };   
            }
        }

        /// <inheritdoc/>
        public event NotifyCollectionChangedEventHandler CollectionChanged;

        /// <inheritdoc/>
        public event PropertyChangedEventHandler PropertyChanged;

        /// <inheritdoc/>
        public int Count
        {
            get
            {
                return this.collection.Count;
            }
        }

        /// <inheritdoc/>
        public bool IsReadOnly
        {
            get
            {
                return true;
            }
        }

        /// <inheritdoc/>
        public object SyncRoot
        {
            get
            {
                return ((ICollection)this.collection).SyncRoot;
            }
        }

        /// <inheritdoc/>
        public bool IsSynchronized
        {
            get
            {
                return ((ICollection)this.collection).IsSynchronized;
            }
        }

        /// <inheritdoc/>
        public void Add(T item)
        {
            throw new CollectionReadOnlyException();
        }

        /// <inheritdoc/>
        public void Clear()
        {
            throw new CollectionReadOnlyException();
        }

        /// <inheritdoc/>
        public bool Contains(T item)
        {
            return this.collection.Contains(item);
        }

        /// <inheritdoc/>
        public void CopyTo(T[] array, int arrayIndex)
        {
            this.collection.CopyTo(array, arrayIndex);
        }

        /// <inheritdoc/>
        IEnumerator IEnumerable.GetEnumerator()
        {
            return this.GetEnumerator();
        }

        /// <inheritdoc/>
        public IEnumerator<T> GetEnumerator()
        {
            return this.collection.GetEnumerator();
        }

        /// <inheritdoc/>
        public void CopyTo(Array array, int index)
        {
            ((ICollection)this.collection).CopyTo(array, index);
        }

        /// <inheritdoc/>
        public bool Remove(T item)
        {
            throw new CollectionReadOnlyException();
        }
    }
}
