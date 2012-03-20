namespace GeoGen.Studio.Utilities.Collections
{
    using GeoGen.Studio.Utilities.Collections.Testing;

    using NUnit.Framework;

    // ReSharper disable InconsistentNaming

    /// <summary>
    /// Tests for the <see cref="PriorityObservableCollection"/> class.
    /// </summary>
    [TestFixture]
    class PriorityObservableCollectionTests
    {
        /// <summary>
        /// Tests that Add correctly sorts in newly added item.
        /// </summary>
        [Test]
        public void Add_MiddleItem_SortsIn()
        {
            var collection = new PriorityObservableCollection
            {
                new TestingPriorityObservable(3),
                new TestingPriorityObservable(1),
                new TestingPriorityObservable(2)
            };

            Assert.AreEqual(2, collection[1].Priority);
        }

        /// <summary>
        /// Tests that changing item's priority after adding it correctly updates the collection order.
        /// </summary>
        [Test]
        public void Add_ChangePriority_ChangesOrder()
        {
            var item = new TestingPriorityObservable(1);
            var collection = new PriorityObservableCollection
            {
                new TestingPriorityObservable(3),
                new TestingPriorityObservable(2),
                item,
            };

            item.Priority = 2.5;

            Assert.AreEqual(2.5, collection[1].Priority);
        }

        /// <summary>
        /// Tests that the collection ignores changes.
        /// </summary>
        [Test]
        public void Add_ChangeNotPriority_Ignores()
        {
            var item = new TestingExtendedPriorityObservable
            {
                Priority = 1
            };

            var collection = new PriorityObservableCollection
            {
                new TestingPriorityObservable(3),
                new TestingPriorityObservable(2),
                item,
            };

            item.NotPriority = 2.5;

            Assert.AreNotEqual(2.5, collection[1].Priority);            
        }

        /// <summary>
        /// Tests that generic Add correctly sorts in newly added item.
        /// </summary>
        [Test]
        public void GenericAdd_MiddleItem_SortsIn()
        {
            var collection = new PriorityObservableCollection<TestingPriorityObservable>
            {
                new TestingPriorityObservable(3),
                new TestingPriorityObservable(1),
                new TestingPriorityObservable(2)
            };

            Assert.AreEqual(2, collection[1].Priority);
        }

        /// <summary>
        /// Tests that changing item's priority after adding it correctly updates the generic collection order.
        /// </summary>
        [Test]
        public void GenericAdd_ChangePriority_ChangesOrder()
        {
            var item = new TestingPriorityObservable(1);
            var collection = new PriorityObservableCollection<TestingPriorityObservable>
            {
                new TestingPriorityObservable(3),
                new TestingPriorityObservable(2),
                item,
            };

            item.Priority = 2.5;

            Assert.AreEqual(2.5, collection[1].Priority);
        }

        /// <summary>
        /// Tests that the generic collection ignores changes.
        /// </summary>
        [Test]
        public void GenericAdd_ChangeNotPriority_Ignores()
        {
            var item = new TestingExtendedPriorityObservable
            {
                Priority = 1
            };

            var collection = new PriorityObservableCollection<TestingPriorityObservable>
            {
                new TestingPriorityObservable(3),
                new TestingPriorityObservable(2),
                item,
            };

            item.NotPriority = 2.5;

            Assert.AreNotEqual(2.5, collection[1].Priority);
        }
    }
}

