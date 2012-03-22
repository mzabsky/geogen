namespace GeoGen.Studio.Utilities.Context
{
    using NUnit.Framework;

    // ReSharper disable InconsistentNaming

    [TestFixture]
    class ContextTests
    {
        [Test]
        public void Context_Construct_LabelsMatch()
        {
            var label = "test";
            var context = new Context(label);
            Assert.AreEqual(context.Label, label);
        }

        [Test]
        public void Equals_OneContext_True()
        {
            var context = new Context("test");
            Assert.IsTrue(context.Equals(context));
        }

        [Test]
        public void Equals_SameLabel_False()
        {
            var context1 = new Context("test");
            var context2 = new Context("test");
            Assert.IsFalse(context1.Equals(context2));
        }
    }
}
