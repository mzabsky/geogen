namespace GeoGen.Studio.Utilities.Collections
{
    using System.Collections;
    using System.Collections.Generic;

    using NUnit.Framework;

    // ReSharper disable InconsistentNaming

    /// <summary>
    /// Tests for the <see cref="UniqueObservableQueue{TValue}"/> class.
    /// </summary>
    [TestFixture]
    class UniqueObservableQueueTests
    {
        /// <summary>
        /// Tests that Add adds the item to the beginning of the collection if the item was not present in the collection yet.
        /// </summary>
        [Test]
        public void Add_UnknownItem_AddsToBeginning()
        {
            var collection = new UniqueObservableQueue<int>
            {
                3,
                2,
                1
            };

            Assert.AreEqual(1, collection[0]);
        }

        /// <summary>
        /// Tests that Add adds item which is already in the collection.
        /// </summary>
        [Test]
        public void Add_RepeatedItem_AddsToBeginning()
        {
            var collection = new UniqueObservableQueue<int>
            {
                3,
                2,
                3
            };

            Assert.AreEqual(3, collection[0]);
        }


        /// <summary>
        /// Tests that Add adds item which is already in the collection.
        /// </summary>
        [Test]
        public void Add_RepeatedItem_RemovesPrevious()
        {
            var collection = new UniqueObservableQueue<int>
            {
                3,
                2,
                3
            };

            Assert.AreEqual(2, collection.Count);
        }

        /// <summary>
        /// Tests that Add does nothing when the iem is already in the collection on the first place.
        /// </summary>
        [Test]
        public void Add_RepeatedItemAlreadyFirst_DoesNothing()
        {
            var collection = new UniqueObservableQueue<int>
            {
                3,
                2,
                2
            };

            Assert.AreEqual(2, collection[0]);
        }

        /// <summary>
        /// Tests that Add does nothing when the iem is already in the collection on the first place.
        /// </summary>
        [Test]
        public void Add_EquivalentItemWithCustonComparer_RemovesEquivalent()
        {
            // The items' modulo 5 has to be the same
            var comparer = new DelegateEqualityComparer<int>((a, b) => a % 5 == b % 5);

            var collection = new UniqueObservableQueue<int>(comparer)
            {
                3,
                4,
                8
            };

            Assert.AreEqual(2, collection.Count);
        }

        /// <summary>
        /// Tests that CopyTo copies the items correctly.
        /// </summary>
        [Test]
        public void CopyTo_Items_Copies()
        {
            var collection = new UniqueObservableQueue<int>
            {
                3,
                2,
                1
            };

            var array = new []{0, 0, 0, 0};

            collection.CopyTo(array, 1);

            Assert.AreEqual(2, array[2]);
        }

        /// <summary>
        /// Tests that Remove removes the item from the collection.
        /// </summary>
        [Test]
        public void Remove_Item_Removes()
        {
            var collection = new UniqueObservableQueue<int>
            {
                3,
                2,
                1
            };

            collection.Remove(2);

            Assert.AreEqual(3, collection[1]);
        }

        /// <summary>
        /// Tests that Clear removes all items from the collection.
        /// </summary>
        [Test]
        public void Clear_NonemptyCollection_RemovesAll()
        {
            var collection = new UniqueObservableQueue<int>
            {
                3,
                2,
                1
            };

            collection.Clear();

            Assert.AreEqual(0, collection.Count);
        }

        /// <summary>
        /// Tests that Contains returns true when the item is in the collection.
        /// </summary>
        [Test]
        public void Contains_ContainedItem_ReturnsTrue()
        {
            var collection = new UniqueObservableQueue<int>
            {
                3,
                2,
                1
            };

            Assert.IsTrue(collection.Contains(2));
        }

        /// <summary>
        /// Tests that Contains returns false when the item is in the collection.
        /// </summary>
        [Test]
        public void Contains_ContainedItem_ReturnsFalse()
        {
            var collection = new UniqueObservableQueue<int>
            {
                3,
                2,
                1
            };

            Assert.IsFalse(collection.Contains(4));
        }

        /// <summary>
        /// Tests that generic enumerator walks through all the items in the collection.
        /// </summary>
        [Test]
        public void GetEnumerator_Collection_ReturnsCorrectCollection()
        {
            var collection = new UniqueObservableQueue<int>
            {
                3,
                2,
                1
            };

            // Don't use LINQ may not or in the future do some smart optimization to avoid the enumerator
            var result = new List<int>();
            foreach (int item in collection)
            {
                result.Add(item);
            }

            var expectedResult = new List<int>
            {
                1,
                2,
                3
            };

            Assert.That(expectedResult, Is.EquivalentTo(result));
        }

        /// <summary>
        /// Tests that non-generic enumerator walks through all the items in the collection.
        /// </summary>
        [Test]
        public void EnumerableGetEnumerator_Collection_ReturnsCorrectCollection()
        {
            var collection = (IEnumerable)new UniqueObservableQueue<int>
            {
                3,
                2,
                1
            };

            // Don't use LINQ may not or in the future do some smart optimization to avoid the enumerator
            var result = new List<int>();
            foreach (int item in collection)
            {
                result.Add(item);
            }

            var expectedResult = new List<int>
            {
                1,
                2,
                3
            };

            Assert.That(expectedResult, Is.EquivalentTo(result));
        }
    }
}

