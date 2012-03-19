namespace GeoGen.Studio.PlugInLoader
{
    using System;

    using NUnit.Framework;

    using Rhino.Mocks;

    // ReSharper disable InconsistentNaming

    /// <summary>
    /// Tests for the <see cref="RegistratorComparer"/> class.
    /// </summary>
    [TestFixture]
    public class RegistratorComparerTests
    {
        /// <summary>
        /// Tests that Compare returns correct results on registrators in various relationships.
        /// </summary>
        /// <param name="producedA">Interface produced by A.</param>
        /// <param name="consumedA">Interface consumed by A.</param>
        /// <param name="producedB">Interface produced by B.</param>
        /// <param name="consumedB">Interface consumed by B.</param>
        /// <param name="expectedResult">The expected result.</param>
        [TestCase(typeof(int), typeof(string), typeof(float), typeof(double), 0)] // Unrelated registrators -> A == B.
        [TestCase(typeof(int), typeof(float), typeof(float), typeof(double), 1)] // A consumes interface provided by B -> A >= B.
        [TestCase(typeof(int), typeof(string), typeof(float), typeof(int), -1)] // A provides interface consumed by B -> A <= B.
        [TestCase(typeof(int), typeof(string), typeof(string), typeof(int), 0, ExpectedException = typeof(InvalidOperationException))] // Cyclic dependency.
        public void Compare_UncomparableRegistrators_ReturnsEquals(Type producedA, Type consumedA, Type producedB, Type consumedB, int expectedResult)
        {
            var registratorA = MockRepository.GenerateStub<IRegistrator>();
            registratorA.Stub(p => p.ImplementedInterfaces).Return(new[] { producedA });
            registratorA.Stub(p => p.DependsOn).Return(new[] { consumedA });

            var registratorB = MockRepository.GenerateStub<IRegistrator>();
            registratorB.Stub(p => p.ImplementedInterfaces).Return(new[] { producedB });
            registratorB.Stub(p => p.DependsOn).Return(new[] { consumedB });

            var comparer = new RegistratorComparer();
            var result = comparer.Compare(registratorA, registratorB);

            Assert.AreEqual(expectedResult, result);
        }
    }
}