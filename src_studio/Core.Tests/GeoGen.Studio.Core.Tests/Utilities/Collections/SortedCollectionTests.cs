namespace GeoGen.Studio.Utilities.Collections
{
    using System;
    using System.Collections;
    using System.Collections.Generic;

    using NUnit.Framework;

    using Rhino.Mocks;

    // ReSharper disable InconsistentNaming
    /// <summary>
    /// Tests for the <see cref="SortedCollection{TValue}"/> class.
    /// </summary>
    [TestFixture]
    public class SortedCollectionTests
    {
        /// <summary>
        /// Tests that constructor correctly sets comparer.
        /// </summary>
        [Test]
        public void Constructor_Comparer_Sets()
        {
            var comparer = MockRepository.GenerateMock<IComparer<int>>();

            var collection = new SortedCollection<int>(comparer);

            Assert.AreSame(comparer, collection.Comparer);
        }

        /// <summary>
        /// Tests that reduction of capacity precisely to number of items is allowed.
        /// </summary>
        [Test]
        public void Capacity_ReduceToCount_Sets()
        {
            var collection = new SortedCollection<int>
            {
                1
            };

            collection.Capacity = 1;

            Assert.AreEqual(1, collection.Capacity);
        }

        /// <summary>
        /// Tests that changing capacity to capacity does nothing.
        /// </summary>
        [Test]
        public void Capacity_SetToCapacity_DoesNothing()
        {
            var collection = new SortedCollection<int>
            {
                1
            };

            int capacity = collection.Capacity;
            collection.Capacity = capacity;

            Assert.AreEqual(capacity, collection.Capacity);
        }

        /// <summary>
        /// Tests that capacity can be arbitrarily expanded.
        /// </summary>
        [Test]
        public void Capacity_Expand_Sets()
        {
            var collection = new SortedCollection<int>();
            collection.Capacity = 67;

            Assert.AreEqual(67, collection.Capacity);
        }

        /// <summary>
        /// Tests that capacity expansion doesn't break values.
        /// </summary>
        [Test]
        public void Capacity_Expand_KeepsValues()
        {
            var collection = new SortedCollection<int>
            {
                1,
                2,
                3
            };

            collection.Capacity = 67;

            Assert.AreEqual(2, collection[1]);
        }

        /// <summary>
        /// Tests that capacity can be reduced back to zero after it was expanded.
        /// </summary>
        [Test]
        public void Capacity_ExpandedReduceTo0_Sets()
        {
            var collection = new SortedCollection<int>();
            collection.Capacity = 67;
            collection.Capacity = 0;

            Assert.AreEqual(0, collection.Capacity);
        }

        /// <summary>
        /// Tests that reducing capacity below number of items in the collection will throw exception.
        /// </summary>
        [Test]
        [ExpectedException(typeof(ArgumentException))]
        public void Capacity_ReduceBelowCount_ThrowsException()
        {
            var collection = new SortedCollection<int>
            {
                1,
                2,
                3
            };

            collection.Capacity = 2;
        }

        /// <summary>
        /// Tests that default comparer is used when none is passed.
        /// </summary>
        [Test]
        public void Comparer_Unset_UsesDefault()
        {
            var collection = new SortedCollection<int>();

            Assert.AreEqual(collection.Comparer, Comparer<int>.Default);
        }

        /// <summary>
        /// Tests that FixedSize is true.
        /// </summary>
        [Test]
        public void IsFixedSize_True()
        {
            var collection = (IList)new SortedCollection<int>();

            Assert.IsTrue(collection.IsFixedSize);
        }

        /// <summary>
        /// Tests that IList.IsReadOnly is true.
        /// </summary>
        [Test]
        public void IListIsReadOnly_False()
        {
            var collection = (IList)new SortedCollection<int>();

            Assert.IsFalse(collection.IsReadOnly);
        }

        /// <summary>
        /// Tests that ICollection.IsReadOnly is true.
        /// </summary>
        [Test]
        public void ICollectionIsReadOnly_False()
        {
            var collection = (ICollection<int>)new SortedCollection<int>();

            Assert.IsFalse(collection.IsReadOnly);
        }

        /// <summary>
        /// Tests that IsSynchronized is true.
        /// </summary>
        [Test]
        public void IsSynchronized_False()
        {
            var collection = (ICollection)new SortedCollection<int>();

            Assert.IsFalse(collection.IsSynchronized);
        }

        /// <summary>
        /// Tests that SyncRoot is not null.
        /// </summary>
        [Test]
        public void SyncRoot_NotNull()
        {
            var collection = (ICollection)new SortedCollection<int>();

            Assert.AreNotEqual(null, collection.SyncRoot);
        }

        /// <summary>
        /// Tests that generic indexer correctly returns value when the index is within bounds of the collection.
        /// </summary>
        [Test]
        public void Indexer_ValidIndexGet_ReturnsValue()
        {
            var collection = new SortedCollection<int>
            {
                1,
                2,
                3
            };

            Assert.AreEqual(2, collection[1]);
        }

        /// <summary>
        /// Tests that generic read indexer throws exception value when the index is out of bounds of the collection.
        /// </summary>
        [TestCase(-5)]
        [TestCase(67)]
        [ExpectedException(typeof(ArgumentOutOfRangeException))]
        public void Indexer_OutOfBoundsIndexGet_ThrowsException(int index)
        {
            var collection = new SortedCollection<int>();

            var result = collection[index];
        }

        /// <summary>
        /// Tests that generic indexer correctly writes value when the index is within bounds of the collection.
        /// </summary>
        [Test]
        public void Indexer_ValidIndexSet_WritesValue()
        {
            var collection = new SortedCollection<int>
            {
                1,
                2,
                3
            };

            collection[1] = 4;

            Assert.AreEqual(4, collection[1]);
        }

        /// <summary>
        /// Tests that generic write indexer throws exception value when the index is out of bounds of the collection.
        /// </summary>
        [TestCase(-5)]
        [TestCase(67)]
        [ExpectedException(typeof(ArgumentOutOfRangeException))]
        public void Indexer_OutOfBoundsIndexSet_ThrowsException(int index)
        {
            var collection = new SortedCollection<int>();

            collection[index] = 4;
        }

        /// <summary>
        /// Tests that <see cref="IList"/> non-generic indexer also correctly returns value. Exception testing is done by tests above - the code is shared.
        /// </summary>
        [Test]
        public void IListIndexer_Get_ReturnsValue()
        {
            var collection = (IList)new SortedCollection<int>
            {
                1,
                2,
                3
            };

            Assert.AreEqual(2, collection[1]);
        }

        /// <summary>
        /// Tests that <see cref="IList"/> non-generic indexer also correctly writes value. Exception testing is done by tests above - the code is shared.
        /// </summary>
        [Test]
        public void IListIndexer_Set_WritesValue()
        {
            var collection = (IList)new SortedCollection<int>
            {
                1,
                2,
                3
            };

            collection[1] = 4;

            Assert.AreEqual(4, collection[1]);
        }

        /// <summary>
        /// Tests that adding null throws exception.
        /// </summary>
        [Test]
        [ExpectedException(typeof(ArgumentNullException))]
        public void Add_Null_ThrowsException()
        {
            new SortedCollection<ObservableObject>
            {
                null
            };
        }

        /// <summary>
        /// Tests that adding to empty collection adds.
        /// </summary>
        [Test]
        public void Add_ItemToEmpty_Adds()
        {
            var collection = new SortedCollection<int>
            {
                1
            };

            Assert.AreEqual(1, collection.Count);
        }

        /// <summary>
        /// Tests that value is correctly sorted between two values if it belongs there.
        /// </summary>
        [Test]
        public void Add_BetweenTwo_Sorts()
        {
            var collection = new SortedCollection<int>
            {
                1,
                3,
                2
            };

            Assert.AreEqual(2, collection[1]);
        }

        /// <summary>
        /// Tests that value is correctly sorted before other elements if it belongs there.
        /// </summary>
        [Test]
        public void Add_BeforeFirst_Sorts()
        {
            var collection = new SortedCollection<int>
            {
                2,
                3,
                1
            };

            Assert.AreEqual(1, collection[0]);
        }

        /// <summary>
        /// Tests that custom comparer is used in sorting.
        /// </summary>
        [Test]
        public void Add_CustomComparer_Sorts()
        {
            // Use reverse default comparer
            var collection = new SortedCollection<int>(
                new DelegateComparer<int>(
                    (a, b) => -Comparer<int>.Default.Compare(a, b)))
            {
                2,
                3,
                2
            };

            Assert.AreEqual(3, collection[0]);
        }

        /// <summary>
        /// Tests that <see cref="IList"/> non-generic Add also adds, deailed testing is done for the generic add (code is shared).
        /// </summary>
        [Test]
        public void IListAdd_Item_Adds()
        {
            var collection = (IList)new SortedCollection<int>();
            collection.Add(1);
            collection.Add(2);
            collection.Add(3);

            Assert.AreEqual(2, collection[1]);
        }

        /// <summary>
        /// Tests that Clear clears.
        /// </summary>
        [Test]
        public void Clear_NonemptyCollection_Clears()
        {
            var collection = new SortedCollection<int>
            {
                2,
                3,
                1
            };

            collection.Clear();

            Assert.AreEqual(0, collection.Count);
        }

        /// <summary>
        /// Tests that Contains works.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="expectedResult">Expected result of Contains.</param>
        [TestCase(2, true)]
        [TestCase(4, false)]
        public void Contains_Item_Works(int value, bool expectedResult)
        {
            var collection = new SortedCollection<int>
            {
                2,
                3,
                1
            };

            bool result = collection.Contains(value);

            Assert.AreEqual(expectedResult, result);
        }

        /// <summary>
        /// Tests that Contains works in the non-generic variant a well.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="expectedResult">Expected result of Contains.</param>
        [TestCase(2, true)]
        [TestCase(4, false)]
        public void IListContains_Item_Works(int value, bool expectedResult)
        {
            var collection = (IList)new SortedCollection<int>
            {
                2,
                3,
                1
            };

            bool result = collection.Contains(value);

            Assert.AreEqual(expectedResult, result);
        }

        /// <summary>
        /// Tests that CopyTo copies items correctly when passed correct parameters.
        /// </summary>
        [Test]
        public void CopyTo_ValidRange_Copies()
        {
            var collection = new SortedCollection<int>
            {
                1,
                2,
                3
            };

            var array = new[] { 6, 7, 8, 9 };

            collection.CopyTo(array, 1);

            Assert.AreEqual(3, array[3]);
        }

        /// <summary>
        /// Tests that CopyTo copies items correctly when passed correct parameters using the non-generic CopyTo.
        /// </summary>
        [Test]
        public void IListCopyTo_ValidRange_Copies()
        {
            var collection = (IList)new SortedCollection<int>
            {
                1,
                2,
                3
            };

            var array = new[] { 6, 7, 8, 9 };

            collection.CopyTo(array, 1);

            Assert.AreEqual(3, array[3]);
        }

        /// <summary>
        /// Tests that IndexOf on null throws exception.
        /// </summary>
        [Test]
        [ExpectedException(typeof(ArgumentNullException))]
        public void IndexOf_Null_ThrowsException()
        {
            var collection = new SortedCollection<ObservableObject>();
            collection.IndexOf(null);
        }

        /// <summary>
        /// Tests IndexOf for a value which exists and which does not exist in the array, expecting correct result.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="expectedResult">The expected result.</param>
        [TestCase(2, 1)]
        [TestCase(4, -1)]
        public void IndexOf_Value_ReturnsIndex(int value, int expectedResult)
        {
            var collection = new SortedCollection<int>
            {
                1,
                2,
                3
            };

            int result = collection.IndexOf(value);

            Assert.AreEqual(expectedResult, result);
        }

        /// <summary>
        /// Tests that non-generic IndexOf works as well.
        /// </summary>
        [Test]
        public void IListIndexOf_Value_ReturnsIndex()
        {
            var collection = (IList)new SortedCollection<int>
            {
                1,
                2,
                3
            };

            int result = collection.IndexOf(2);

            Assert.AreEqual(1, result);
        }

        /// <summary>
        /// Tests that inserting null throws exception.
        /// </summary>
        [Test]
        [ExpectedException(typeof(ArgumentNullException))]
        public void Insert_Null_ThrowsException()
        {
            var collection = new SortedCollection<ObservableObject>();
            collection.Insert(0, null);
        }

        /// <summary>
        /// Tests that out of range index throws exception.
        /// </summary>
        [TestCase(-1)]
        [TestCase(67)]
        [ExpectedException(typeof(ArgumentOutOfRangeException))]
        public void Insert_OutOfRangeIndex_ThrowsException(int index)
        {
            var collection = new SortedCollection<int>();
            collection.Insert(index, 5);
        }

        /// <summary>
        /// Tests that valid item and index get inserted.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="index">The index.</param>
        [TestCase(1, 0)]
        [TestCase(1, 1)]
        [TestCase(1, 3)]
        public void Insert_ValidItemAndIndex_Inserts(int value, int index)
        {
            var collection = new SortedCollection<int>
            {
                1,
                2,
                3
            };

            collection.Insert(index, value);

            Assert.AreEqual(collection[index], value);
        }

        /// <summary>
        /// Tests that valid item with valid index gets inserted with non-generic insert.
        /// </summary>
        [Test]
        public void IListInsert_ValidItem_Inserts()
        {
            var collection = (IList)new SortedCollection<int>
            {
                1,
                2,
                3
            };

            collection.Insert(1, 5);

            Assert.AreEqual(collection[1], 5);
        }

        /// <summary>
        /// Tests that true gets returned as long as the item being removed exists in the collection.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="expectedResult">The expected result.</param>
        [TestCase(2, true)]
        [TestCase(4, false)]
        public void Remove_Item_ReturnsTrueIfExists(int value, bool expectedResult)
        {
            var collection = new SortedCollection<int>
            {
                1,
                2,
                3
            };

            bool result = collection.Remove(value);

            Assert.AreEqual(expectedResult, result);
        }

        /// <summary>
        /// Tests that item removed with Remove is actually removed from the collection.
        /// </summary>
        [Test]
        public void Remove_ExistingItem_Removes()
        {
            var collection = new SortedCollection<int>
            {
                1,
                2,
                3
            };

            collection.Remove(2);

            Assert.AreNotEqual(2, collection[1]);
        }

        /// <summary>
        /// Tests that non-generic remove works as well.
        /// </summary>
        [Test]
        public void IListRemove_ExistingItem_Removes()
        {
            var collection = (IList)new SortedCollection<int>
            {
                1,
                2,
                3
            };

            collection.Remove(2);

            Assert.AreNotEqual(2, collection[1]);
        }

        /// <summary>
        /// Tests that RemoveAt throws exception on incorrect index.
        /// </summary>
        /// <param name="index">The index.</param>
        [TestCase(-1)]
        [TestCase(67)]
        [ExpectedException(typeof(ArgumentOutOfRangeException))]
        public void RemoveAt_OutOfRange_ThrowsException(int index)
        {
            var collection = (IList)new SortedCollection<int>();

            collection.RemoveAt(index);
        }

        /// <summary>
        /// Tests that RemoveAt removed item on correct index.
        /// </summary>
        [Test]
        public void RemoveAt_ValidIndex_Removes()
        {
            var collection = (IList)new SortedCollection<int>
            {
                1,
                2,
                3
            };

            collection.RemoveAt(1);

            Assert.AreNotEqual(2, collection[1]);
        }

        /// <summary>
        /// Tests that generic enumerator walks through all the items in the collection.
        /// </summary>
        [Test]
        public void GetEnumerator_Collection_ReturnsCorrectCollection()
        {
            var collection = new SortedCollection<int>
            {
                3,
                1,
                2
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
        /// Tests that non-generic enumerator works as well.
        /// </summary>
        [Test]
        public void IEnumerableGetEnumerator_Collection_CorrectCount()
        {
            var collection = (IEnumerable)new SortedCollection<int>
            {
                3,
                1,
                2
            };

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
