namespace GeoGen.Studio.Utilities.Collections
{
    using NUnit.Framework;

    using Rhino.Mocks;

    // ReSharper disable InconsistentNaming

    /// <summary>
    /// Tests for the <see cref="PriorityComparer"/> class.
    /// </summary>
    [TestFixture]
    public class PriorityComparerTests
    {
        /// <summary>
        /// Tests that <see cref="PriorityComparer"/> sorts in descending order by <see cref="IPriority.Priority"/>.
        /// </summary>
        /// <param name="priorityA">The priority A.</param>
        /// <param name="priorityB">The priority B.</param>
        /// <param name="expectedResult">The expected result.</param>
        [TestCase(1, 2, 1)]
        [TestCase(1, 1, 0)]
        [TestCase(2, 1, -1)]
        public void Equals_TwoValues_Compare(double priorityA, double priorityB, int expectedResult)
        {
            var comparer = new PriorityComparer();

            var priorityImplementationA = MockRepository.GenerateStub<IPriority>();
            priorityImplementationA.Stub(p => p.Priority).Return(priorityA);

            var priorityImplementationB = MockRepository.GenerateStub<IPriority>();
            priorityImplementationB.Stub(p => p.Priority).Return(priorityB);

            var result = comparer.Compare(priorityImplementationA, priorityImplementationB);

            Assert.AreEqual(expectedResult, result);
        }
    }
}
