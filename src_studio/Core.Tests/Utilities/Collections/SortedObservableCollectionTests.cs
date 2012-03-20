namespace GeoGen.Studio.Utilities.Collections
{
    using System.Collections.Specialized;
    using System.ComponentModel;
    using System.Linq;

    using GeoGen.Studio.Utilities.Collections.Testing;

    using NUnit.Framework;

    // ReSharper disable InconsistentNaming
    /// <summary>
    /// Tests for the <see cref="SortedObservableCollection{TValue}"/> class.
    /// </summary>
    [TestFixture]
    class SortedObservableCollectionTests
    {
        /// <summary>
        /// Compares two <see cref="NotifyCollectionChangedEventArgs"/> instances.
        /// </summary>
        /// <param name="expected">First <see cref="System.Collections.Specialized.NotifyCollectionChangedEventArgs"/>.</param>
        /// <param name="fired">Second <see cref="System.Collections.Specialized.NotifyCollectionChangedEventArgs"/>.</param>
        /// <returns><c>true</c> if the instances are equal.</returns>
        public static bool AreCollectionChangedArgsEqual(NotifyCollectionChangedEventArgs expected, NotifyCollectionChangedEventArgs fired)
        {
            if (expected.Action != fired.Action)
            {
                return false;
            }

            if (expected.NewStartingIndex != fired.NewStartingIndex)
            {
                return false;
            }

            if (expected.OldStartingIndex != fired.OldStartingIndex)
            {
                return false;
            }

            bool newItemsMatch = true;
            if (expected.NewItems != null)
            {
                newItemsMatch = expected.NewItems.Cast<IPriority>().Zip
                    (fired.NewItems.Cast<IPriority>(), (a, b) => new { Expected = a, Fired = b }).All
                    (a => a.Expected == a.Fired);
            }
            
            bool oldItemsMatch = true;
            if (expected.OldItems != null)
            {
                oldItemsMatch = expected.OldItems.Cast<IPriority>().Zip
                    (fired.OldItems.Cast<IPriority>(), (a, b) => new { Expected = a, Fired = b }).All
                    (a => a.Expected == a.Fired);
            }

            return newItemsMatch && oldItemsMatch;
        }

        /// <summary>
        /// Tests that Add correctly triggers the CollectionChanged event.
        /// </summary>
        [Test]
        public void Add_Item_FiresCollectionChanged()
        {
            var collection = new TestingSortedObservableCollection
            {
                new TestingPriorityObservable(),
                new TestingPriorityObservable(),
                new TestingPriorityObservable()
            };

            NotifyCollectionChangedEventArgs firedArgs = null;
            collection.CollectionChanged += delegate(object sender, NotifyCollectionChangedEventArgs args)
            {
                firedArgs = args;
            };

            var item = new TestingPriorityObservable();
            collection.Add(item);

            var expectedArgs = new NotifyCollectionChangedEventArgs(
                action: NotifyCollectionChangedAction.Add,
                changedItem: item,
                index: 3);

            Assert.IsTrue(AreCollectionChangedArgsEqual(expectedArgs, firedArgs));
        }

        /// <summary>
        /// Tests that Insert correctly fires the CollectionChanged event.
        /// </summary>
        [Test]
        public void Insert_Item_FiresCollectionChanged()
        {
            var collection = new TestingSortedObservableCollection
            {
                new TestingPriorityObservable(),
                new TestingPriorityObservable(),
                new TestingPriorityObservable()
            };

            NotifyCollectionChangedEventArgs firedArgs = null;
            collection.CollectionChanged += delegate(object sender, NotifyCollectionChangedEventArgs args)
            {
                firedArgs = args;
            };

            var item = new TestingPriorityObservable();
            collection.Insert(1, item);

            var expectedArgs = new NotifyCollectionChangedEventArgs(
                action: NotifyCollectionChangedAction.Add,
                changedItem: item,
                index: 1);

            Assert.IsTrue(AreCollectionChangedArgsEqual(expectedArgs, firedArgs));
        }

        /// <summary>
        /// Tests that Remove correctly fires the CollectionChanged event.
        /// </summary>
        [Test]
        public void Remove_Item_FiresCollectionChanged()
        {
            var item = new TestingPriorityObservable();
            var collection = new TestingSortedObservableCollection
            {                
                new TestingPriorityObservable(),
                item,
                new TestingPriorityObservable()
            };

            NotifyCollectionChangedEventArgs firedArgs = null;
            collection.CollectionChanged += delegate(object sender, NotifyCollectionChangedEventArgs args)
            {
                firedArgs = args;
            };

            collection.Remove(item);

            var expectedArgs = new NotifyCollectionChangedEventArgs(
                action: NotifyCollectionChangedAction.Remove,
                changedItem: item,
                index: 1);

            Assert.IsTrue(AreCollectionChangedArgsEqual(expectedArgs, firedArgs));
        }

        /// <summary>
        /// Tests that RemoveAt correctly fires the CollectionChanged event.
        /// </summary>
        [Test]
        public void RemoveAt_Item_FiresCollectionChanged()
        {
            var item = new TestingPriorityObservable();
            var collection = new TestingSortedObservableCollection
            {                
                new TestingPriorityObservable(),
                item,
                new TestingPriorityObservable()
            };

            NotifyCollectionChangedEventArgs firedArgs = null;
            collection.CollectionChanged += delegate(object sender, NotifyCollectionChangedEventArgs args)
            {
                firedArgs = args;
            };

            collection.RemoveAt(1);

            var expectedArgs = new NotifyCollectionChangedEventArgs(
                action: NotifyCollectionChangedAction.Remove,
                changedItem: item,
                index: 1);

            Assert.IsTrue(AreCollectionChangedArgsEqual(expectedArgs, firedArgs));
        }

        /// <summary>
        /// Tests that Clear correctly fires the CollectionChanged event.
        /// </summary>
        [Test]
        public void Clear_Item_FiresCollectionChanged()
        {
            var collection = new TestingSortedObservableCollection
            {                
                new TestingPriorityObservable(),
                new TestingPriorityObservable()
            };

            NotifyCollectionChangedEventArgs firedArgs = null;
            collection.CollectionChanged += delegate(object sender, NotifyCollectionChangedEventArgs args)
            {
                firedArgs = args;
            };

            collection.Clear();

            var expectedArgs = new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset);

            Assert.IsTrue(AreCollectionChangedArgsEqual(expectedArgs, firedArgs));
        }

        /// <summary>
        /// Tests that Clear correctly fires the CollectionChanged event.
        /// </summary>
        [Test]
        public void Indexer_SetItem_FiresCollectionChanged()
        {
            var collection = new TestingSortedObservableCollection
            {                
                new TestingPriorityObservable(),
                new TestingPriorityObservable(),
                new TestingPriorityObservable()
            };

            NotifyCollectionChangedEventArgs firedArgs = null;
            collection.CollectionChanged += delegate(object sender, NotifyCollectionChangedEventArgs args)
            {
                firedArgs = args;
            };

            var oldItem = collection[1];
            var item = new TestingPriorityObservable();
            collection[1] = item;

            var expectedArgs = new NotifyCollectionChangedEventArgs(
                action: NotifyCollectionChangedAction.Replace,
                newItem: item,
                oldItem: oldItem,
                index: 1);

            Assert.IsTrue(AreCollectionChangedArgsEqual(expectedArgs, firedArgs));
        }

        /// <summary>
        /// Tests that property change in item added with Add is correctly detected.
        /// </summary>
        [Test]
        public void Add_ObservableItem_ChangeDetected()
        {
            var item = new TestingPriorityObservable();
            bool changeDetected = false;
            new TestingSortedObservableCollection(p => changeDetected = true)
            {                
                item
            };

            item.Priority = 5;

            Assert.IsTrue(changeDetected);
        }

        /// <summary>
        /// Tests that property change in item added with Insert is correctly detected.
        /// </summary>
        [Test]
        public void Insert_ObservableItem_ChangeDetected()
        {
            var item = new TestingPriorityObservable();
            bool changeDetected = false;
            var collection = new TestingSortedObservableCollection(p => changeDetected = true);
            collection.Insert(0, item);

            item.Priority = 5;

            Assert.IsTrue(changeDetected);
        }

        /// <summary>
        /// Tests that property change in item added with indexer is correctly detected.
        /// </summary>
        [Test]
        public void Indexer_AddedObservableItem_ChangeDetected()
        {            
            bool changeDetected = false;
            var collection = new TestingSortedObservableCollection(p => changeDetected = true)
            {
                new TestingPriorityObservable(),
                new TestingPriorityObservable(),
                new TestingPriorityObservable()
            };

            var item = new TestingPriorityObservable();
            collection[1] = item;

            item.Priority = 5;

            Assert.IsTrue(changeDetected);
        }

        /// <summary>
        /// Tests that property change in item removed with RemoveAt is correctly ignored.
        /// </summary>
        [Test]
        public void Remove_ObservableItem_ChangeIgnored()
        {
            var item = new TestingPriorityObservable();
            bool changeDetected = false;
            var collection = new TestingSortedObservableCollection(p => changeDetected = true)
            {
                item
            };

            collection.Remove(item);

            item.Priority = 5;

            Assert.IsFalse(changeDetected);
        }

        /// <summary>
        /// Tests that property change in item removed with Clear is correctly ignored.
        /// </summary>
        [Test]
        public void RemoveAt_ObservableItem_ChangeIgnored()
        {
            var item = new TestingPriorityObservable();
            bool changeDetected = false;
            var collection = new TestingSortedObservableCollection(p => changeDetected = true)
            {
                item
            };

            collection.RemoveAt(0);

            item.Priority = 5;

            Assert.IsFalse(changeDetected);
        }


        /// <summary>
        /// Tests that property change in item removed with Clear is correctly ignored.
        /// </summary>
        [Test]
        public void Clear_ObservableItem_ChangeIgnored()
        {
            var item = new TestingPriorityObservable();
            bool changeDetected = false;
            var collection = new TestingSortedObservableCollection(p => changeDetected = true)
            {
                item
            };

            collection.Clear();

            item.Priority = 5;

            Assert.IsFalse(changeDetected);
        }

        /// <summary>
        /// Tests that property change in item removed with Clear is correctly ignored.
        /// </summary>
        [Test]
        public void Indexer_RemoveObservableItem_ChangeIgnored()
        {
            var item = new TestingPriorityObservable();
            bool changeDetected = false;
            var collection = new TestingSortedObservableCollection(p => changeDetected = true)
            {
                new TestingPriorityObservable(),
                item,
                new TestingPriorityObservable()
            };

            collection[1] = new TestingPriorityObservable();

            item.Priority = 5;

            Assert.IsFalse(changeDetected);
        }

        /// <summary>
        /// Tests that changes to Count fire notifications.
        /// </summary>
        [Test]
        public void Count_Change_Notifies()
        {
            var collection = new TestingSortedObservableCollection();

            bool fired = false;
            collection.PropertyChanged += delegate(object sender, PropertyChangedEventArgs args)
            {
                if (args.PropertyName == "Count")
                {
                    fired = true;
                }
            };

            collection.Add(new TestingPriorityObservable());

            Assert.IsTrue(fired);
        }
    }
}

