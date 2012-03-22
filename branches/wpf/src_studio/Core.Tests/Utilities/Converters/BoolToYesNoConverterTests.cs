namespace GeoGen.Studio.Utilities.Converters
{
    using System.Globalization;
    using System.Windows.Data;
    using NUnit.Framework;

    // ReSharper disable InconsistentNaming

    /// <summary>
    /// Tests for <see cref="BoolToYesNoConverter"/> class.
    /// </summary>
    [TestFixture]
    public class BoolToYesNoConverterTests
    {
        /// <summary>
        /// Tests that instance getter really returns valids instance.
        /// </summary>
        [Test]
        public void Instance_ReturnsInstance()
        {
            Assert.IsInstanceOf<BoolToYesNoConverter>(BoolToYesNoConverter.Instance);
        }

        /// <summary>
        /// Tests that <c>true</c> gets converted to "Yes".
        /// </summary>
        [Test]
        public void Convert_True_ReturnsYes()
        {
            IValueConverter converter = new BoolToYesNoConverter();
            string result = (string)converter.Convert(true, typeof(string), null, CultureInfo.CurrentCulture);

            Assert.AreEqual(result, "Yes");
        }

        /// <summary>
        /// Tests that <c>false</c> gets converted to "No".
        /// </summary>
        [Test]
        public void Convert_False_ReturnsNo()
        {
            IValueConverter converter = new BoolToYesNoConverter();
            string result = (string)converter.Convert(false, typeof(string), null, CultureInfo.CurrentCulture);

            Assert.AreEqual(result, "No");
        }

        /// <summary>
        /// Tests that anything else gets converted to "Unknown value".
        /// </summary>
        [Test]
        public void Convert_Invalid_ReturnsInvalid()
        {
            IValueConverter converter = new BoolToYesNoConverter();
            string result = (string)converter.Convert(new object(), typeof(string), null, CultureInfo.CurrentCulture);

            Assert.AreEqual(result, "<Unknown value>");
        }

        /// <summary>
        /// Tests that ConverBack doesn't crash.
        /// </summary>
        [Test]
        public void ConvertBack_Null_NoException()
        {
            IValueConverter converter = new BoolToYesNoConverter();
            converter.ConvertBack(null, typeof(int), null, CultureInfo.CurrentCulture);
        }
    }
}
