namespace GeoGen.Studio.Utilities.Context
{
    using System;
    using System.Collections.Generic;
    using System.Globalization;
    using System.Linq;
    using System.Threading;
    using System.Windows.Threading;

    using GeoGen.Studio.Utilities.Extensions;

    using NUnit.Framework;

    // ReSharper disable InconsistentNaming

    [TestFixture]
    class ContextManagerTests
    {
        [SetUp]
        public void SetUp()
        {
            ContextManager.Dispatcher = Dispatcher.CurrentDispatcher;
        }

        [Test]
        [ExpectedException(typeof(ArgumentNullException))]
        public void EnterContext_Null_ExceptionThrown()
        {
            ContextManager.EnterContext(null);
        }

        [Test]
        public void EnterContext_ReturnValue_True()
        {
            Context context = new Context("test");

            Assert.IsTrue(ContextManager.EnterContext(context));
        }

        [Test]
        public void EnterContext_ReturnValue_False()
        {
            Context context = new Context("test");

            ContextManager.EnterContext(context);
            Assert.IsFalse(ContextManager.EnterContext(context));
        }

        [Test]
        public void EnterContext_ContextChanged_Fired()
        {
            Context context = new Context("test");

            bool fired = false;
            ContextManager.ContextChanged += delegate { fired = true; };

            ContextManager.EnterContext(context);

            ContextManager.Dispatcher.DoEvents();

            Assert.IsTrue(fired);
        }

        [Test]
        public void EnterContext_ContextChanged_OnDispatcherThread()
        {
            Context context = new Context("test");

            Thread thread = null;
            ContextManager.ContextChanged += delegate { thread = Thread.CurrentThread; };

            ContextManager.EnterContext(context);

            ContextManager.Dispatcher.DoEvents();

            Assert.AreEqual(thread, Dispatcher.CurrentDispatcher.Thread);
        }

        [Test]
        [ExpectedException(typeof(ArgumentNullException))]
        public void LeaveContext_Null_ExceptionThrown()
        {
            ContextManager.LeaveContext(null);
        }

        [Test]
        public void LeaveContext_ReturnValue_True()
        {
            Context context = new Context("test");

            ContextManager.EnterContext(context);
            Assert.IsTrue(ContextManager.LeaveContext(context));
        }

        [Test]
        public void LeaveContext_ReturnValue_False()
        {
            Context context = new Context("test");

            Assert.IsFalse(ContextManager.LeaveContext(context));
        }

        [Test]
        public void LeaveContext_ContextChanged_Fired()
        {
            Context context = new Context("test");

            ContextManager.EnterContext(context);

            bool fired = false;
            ContextManager.ContextChanged += delegate { fired = true; };

            ContextManager.LeaveContext(context);

            ContextManager.Dispatcher.DoEvents();

            Assert.IsTrue(fired);
        }

        [Test]
        [ExpectedException(typeof(ArgumentNullException))]
        public void IsContextActive_Null_ExceptionThrown()
        {
            ContextManager.IsContextActive(null);
        }

        [Test]
        public void IsActive_UnenteredContext_Active()
        {
            Context context = new Context("test");

            Assert.IsFalse(ContextManager.IsContextActive(context));
        }

        [Test]
        public void IsActive_EnteredContext_IsActive()
        {
            Context context = new Context("test");

            ContextManager.EnterContext(context);

            Assert.IsTrue(ContextManager.IsContextActive(context));
        }

        [Test]
        public void IsActive_LeftOnlyContext_IsNotActive()
        {
            Context context = new Context("test");

            ContextManager.LeaveContext(context);

            Assert.IsFalse(ContextManager.IsContextActive(context));
        }

        [Test]
        public void IsActive_EnteredAndLeftContext_IsNotActive()
        {
            Context context = new Context("test");

            ContextManager.EnterContext(context);
            ContextManager.LeaveContext(context);

            Assert.IsFalse(ContextManager.IsContextActive(context));
        }

        [Test]
        public void IsActive_EqualLabels_NotSame()
        {
            Context context1 = new Context("test_same");
            Context context2 = new Context("test_same");

            ContextManager.EnterContext(context1);

            Assert.IsFalse(ContextManager.IsContextActive(context2));
        }

        [Test]
        [ExpectedException(typeof(ArgumentNullException))]
        public void GetKnownActiveContexts_Null_ExceptionThrown()
        {
            ContextManager.GetKnownActiveContexts(null);
        }

        [Test]
        public void GetKnownActiveContexts_NoContexts_YieldEmptyCollection()
        {
            var result = ContextManager.GetKnownActiveContexts(Enumerable.Empty<Context>());

            Assert.IsFalse(result.Any());
        }

        [Test]
        public void GetKnownActiveContexts_EnteredContext_YieldsItself()
        {
            var list = new List<Context> { new Context("test") };

            ContextManager.EnterContext(list.First());

            var result = ContextManager.GetKnownActiveContexts(list);

            Assert.IsTrue(list.SequenceEqual(result));
        }

        [Test]
        public void GetKnownActiveContexts_UnenteredContext_YieldsEmptyCollection()
        {
            var list = new List<Context> { new Context("test") };

            var result = ContextManager.GetKnownActiveContexts(list);

            Assert.IsFalse(result.Any());
        }

        [Test]
        public void GetKnownActiveContexts_EnteredAndLeftContext_YieldsEmptyCollection()
        {
            var list = new List<Context> { new Context("test") };

            ContextManager.EnterContext(list.First());
            ContextManager.LeaveContext(list.First());

            var result = ContextManager.GetKnownActiveContexts(list);

            Assert.IsFalse(result.Any());
        }

        [Test]
        public void GetKnownActiveContexts_ManyEnteredContexts_YieldsAllInReverse()
        {
            var list = (from i in Enumerable.Range(0, 100) select new Context(i.ToString(CultureInfo.InvariantCulture))).ToList();

            foreach (var c in list)
            {
                ContextManager.EnterContext(c);
            }

            var result = ContextManager.GetKnownActiveContexts(list).Reverse();

            Assert.That(list, Is.EquivalentTo(result));
        }
    }
}
