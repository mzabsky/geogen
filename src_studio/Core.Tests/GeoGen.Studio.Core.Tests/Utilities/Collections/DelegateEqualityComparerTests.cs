namespace GeoGen.Studio.Utilities.Collections
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    using NUnit.Framework;

    // ReSharper disable InconsistentNaming

    /// <summary>
    /// Tests for the <see cref="DelegateEqualityComparer{T}"/> class.
    /// </summary>
    [TestFixture]
    public class DelegateEqualityComparerTests
    {
        /// <summary>
        /// Tests that <see cref="DelegateEqualityComparer{T}.Equals(T,T)"/> returns <c>true</c> when the delegate returns <c>true</c>.
        /// </summary>
        [Test]
        public void Equals_Equal_ReturnsTrue()
        {
            var comparer = new DelegateEqualityComparer<int>((a, b) => true);

            bool result = comparer.Equals(1, 2);

            Assert.IsTrue(result);
        }

        /// <summary>
        /// Tests that <see cref="DelegateEqualityComparer{T}.Equals(T,T)"/> returns <c>false</c> when the delegate returns <c>false</c>.
        /// </summary>
        [Test]
        public void Equals_Equal_ReturnsFalse()
        {
            var comparer = new DelegateEqualityComparer<int>((a, b) => false);

            bool result = comparer.Equals(1, 2);

            Assert.IsFalse(result);
        }

        /// <summary>
        /// Gets the hash code_ some object_ returns its hashcode.
        /// </summary>
        [Test]
        public void GetHashCode_SomeObject_ReturnsItsHashcode()
        {
            var comparer = new DelegateEqualityComparer<object>((a, b) => false);

            var o = new object();

            var hashCode = comparer.GetHashCode(o);

            Assert.AreEqual(hashCode, o.GetHashCode());
        }
    }
}
