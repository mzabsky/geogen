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
	public abstract class SortedObservableCollection<TValue> : 
		SortedCollection<TValue>, 
		INotifyPropertyChanged, 
		INotifyCollectionChanged
	{
		#region Constructors and Destructors

		/// <summary>
		/// Initializes a new empty instance of the <see cref="SortedObservableCollection&lt;TValue&gt;"/> class.
		/// </summary>
		public SortedObservableCollection() {}

		/// <summary>
		/// Initializes a new empty instance of the <see cref="SortedObservableCollection&lt;TValue&gt;"/> class.
		/// </summary>
		/// <param name="comparer">
		/// The comparer.
		/// </param>
		public SortedObservableCollection(IComparer<TValue> comparer)
			: base(comparer) {}

		#endregion

		// Events
		#region Public Events

		/// <summary>
		/// Occurs when the collection changes.
		/// </summary>
		public event NotifyCollectionChangedEventHandler CollectionChanged;

		/// <summary>
		/// Occurs when a property value changes.
		/// </summary>
		public event PropertyChangedEventHandler PropertyChanged;

		#endregion

		#region Public Indexers

		public override TValue this[int index]
		{
			get
			{
				return base[index];
			}

			set
			{
				var oldItem = base[index];
				this.UnsubscribeItem(oldItem);
				this.SubscribeItem(value);
				base[index] = value;
				this.OnPropertyChanged("Item[]");
				this.OnCollectionChanged(NotifyCollectionChangedAction.Replace, oldItem, value, index);
			}
		}

		#endregion

		#region Public Methods

		public override void Clear()
		{
			foreach (var value in this)
			{
				this.UnsubscribeItem(value);
			}

			base.Clear();
			this.OnCollectionReset();
		}

		public override void Insert(int index, TValue value)
		{
			this.UnsubscribeItem(value);
			base.Insert(index, value);
			this.OnPropertyChanged("Count");
			this.OnPropertyChanged("Item[]");
			this.OnCollectionChanged(NotifyCollectionChangedAction.Add, value, index);
		}

		public override void RemoveAt(int index)
		{
			var item = this[index];
			this.UnsubscribeItem(item);
			base.RemoveAt(index);
			this.OnPropertyChanged("Item[]");
			this.OnPropertyChanged("Count");
			this.OnCollectionChanged(NotifyCollectionChangedAction.Remove, item, index);
		}

		#endregion

		#region Methods

		protected abstract void ItemPropertyChanged(object o, PropertyChangedEventArgs args);

		protected override void SubscribeItem(TValue item)
		{
			if (item is INotifyPropertyChanged)
			{
				(item as INotifyPropertyChanged).PropertyChanged += this.ItemPropertyChanged;
			}
		}

		protected override void UnsubscribeItem(TValue item)
		{
			if (item is INotifyPropertyChanged)
			{
				(item as INotifyPropertyChanged).PropertyChanged -= this.ItemPropertyChanged;
			}
		}

		protected virtual void OnCollectionChanged(NotifyCollectionChangedEventArgs e)
		{
			if (this.CollectionChanged != null)
			{
				this.CollectionChanged(this, e);
			}
		}

		protected virtual void OnPropertyChanged(PropertyChangedEventArgs e)
		{
			if (this.PropertyChanged != null)
			{
				this.PropertyChanged(this, e);
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

		private void OnPropertyChanged(string propertyName)
		{
			this.OnPropertyChanged(new PropertyChangedEventArgs(propertyName));
		}

		#endregion
	}
}