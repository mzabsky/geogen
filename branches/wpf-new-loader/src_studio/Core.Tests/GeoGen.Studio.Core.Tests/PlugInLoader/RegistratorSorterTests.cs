namespace GeoGen.Studio.PlugInLoader
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    using NUnit.Framework;

    using Rhino.Mocks;

    // ReSharper disable InconsistentNaming

    /// <summary>
    /// Tests for <see cref="RegistratorSorter"/> class.
    /// </summary>
    [TestFixture]
    public class RegistratorSorterTests
    {
        /// <summary>
        /// Tests if sorter fed with empty collection resturns empty collection.
        /// </summary>
        [Test]
        public void GetAllRegistrators_Empty_ReturnsEmpty()
        {
            var sorter = new RegistratorSorter(Enumerable.Empty<PlugIn>());

            Assert.IsFalse(sorter.GetAllRegistrators().Any());
        }

        /// <summary>
        /// Tests if sorter fed with empty collection resturns empty collection.
        /// </summary>
        [Test]
        public void GetRootRegistrators_Empty_ReturnsEmpty()
        {
            var sorter = new RegistratorSorter(Enumerable.Empty<PlugIn>());

            Assert.IsFalse(sorter.GetRootRegistrators().Any());
        }

        /// <summary>
        /// Tests if sorter fed with empty collection resturns empty collection.
        /// </summary>
        [Test]
        public void GetOrderedtRegistrators_Empty_ReturnsEmpty()
        {
            var sorter = new RegistratorSorter(Enumerable.Empty<PlugIn>());

            Assert.IsFalse(sorter.GetOrderedRegistrators().Any());
        }

        /// <summary>
        /// Tests that GetOrderedRegistrators correctly orders registrators.
        /// </summary>
        [Test]
        public void GetOrderedRegistrators_Collection_ReturnsOrdered()
        {
            /* Let's have five plug-ins:
             *  1) Implements "int", no registrators.
             *  2) Implements nothing
             *      - Register(int)
             *  3) Implements "string"
             *      - Register(int)
             *  4) Implements nothing
             *      - Register(string)
            */
              
            var rootPlugInRegistrator = MockRepository.GenerateStub<IRegistrator>();
            rootPlugInRegistrator.Stub(p => p.ImplementedInterfaces).Return(new[] { typeof(int) });
        }
    }
}
