namespace GeoGen.Studio.Utilities.Collections
{
    using NUnit.Framework;

    // ReSharper disable InconsistentNaming

    /// <summary>
    /// Tests for the <see cref="DelegateComparer{T}"/> class.
    /// </summary>
    [TestFixture]
    public class DelegateComparerTests
    {
        /// <summary>
        /// Tests that the comparer returns value returned by the delegate.
        /// </summary>
        [TestCase(-1)]
        [TestCase(0)]
        [TestCase(1)]
        public void Equals_Compare_ReturnsValue(int value)
        {
            var comparer = new DelegateComparer<int>((a, b) => value);

            int result = comparer.Compare(1, 2);

            Assert.AreEqual(value, result);
        }
    }
}
