namespace GeoGen.Studio.Utilities.Converters
{
    using System.Globalization;
    using System.Windows;
    using System.Windows.Data;

    using NUnit.Framework;

    // ReSharper disable InconsistentNaming

    /// <summary>
    /// Tests for <see cref="FullPathToFileNameConverter"/> class.
    /// </summary>
    [TestFixture]
    public class FullPathToFileNameConverterTests
    {
        /// <summary>
        /// Tests that instance getter really returns valids instance.
        /// </summary>
        [Test]
        public void Instance_ReturnsInstance()
        {
            Assert.IsInstanceOf<FullPathToFileNameConverter>(FullPathToFileNameConverter.Instance);
        }

        /// <summary>
        /// Tests that valid file path gets converted to the file name.
        /// </summary>
        [Test]
        public void Convert_FullPath_ReturnsFileName()
        {
            IValueConverter converter = new FullPathToFileNameConverter();
            var result = (string)converter.Convert("C:/aaa/bbb.ccc", typeof(string), null, CultureInfo.CurrentCulture);

            Assert.AreEqual("bbb.ccc", result);
        }

        /// <summary>
        /// Tests that valid file path gets converted to the file name.
        /// </summary>
        [Test]
        public void Convert_FullPathWithBackSlashes_ReturnsFileName()
        {
            IValueConverter converter = new FullPathToFileNameConverter();
            var result = (string)converter.Convert("C:\\aaa\\bbb.ccc", typeof(string), null, CultureInfo.CurrentCulture);

            Assert.AreEqual("bbb.ccc", result);
        }

        /// <summary>
        /// Tests that path without slashes gets converted to the that same strung.
        /// </summary>
        [Test]
        public void Convert_NoSlashes_ReturnsFileName()
        {
            IValueConverter converter = new FullPathToFileNameConverter();
            var result = (string)converter.Convert("bbb.ccc", typeof(string), null, CultureInfo.CurrentCulture);

            Assert.AreEqual("bbb.ccc", result);
        }

        /// <summary>
        /// Tests that <c>null</c> collection gets converted to <see cref="Visibility.Collapsed"/>.
        /// </summary>
        [Test]
        public void Convert_Null_ReturnsUnknownValue()
        {
            IValueConverter converter = new FullPathToFileNameConverter();
            var result = (string)converter.Convert(null, typeof(Visibility), null, CultureInfo.CurrentCulture);

            Assert.AreEqual("<Unknown value>", result);
        }

        /// <summary>
        /// Tests that <c>null</c> collection gets converted to <see cref="Visibility.Collapsed"/>.
        /// </summary>
        [Test]
        public void Convert_NonStringObject_ReturnsUnknownValue()
        {
            IValueConverter converter = new FullPathToFileNameConverter();
            var result = (string)converter.Convert(new object(), typeof(Visibility), null, CultureInfo.CurrentCulture);

            Assert.AreEqual("<Unknown value>", result);
        }

        /// <summary>
        /// Tests that ConverBack doesn't crash.
        /// </summary>
        [Test]
        public void ConvertBack_Null_NoException()
        {
            IValueConverter converter = new FullPathToFileNameConverter();
            converter.ConvertBack(null, typeof(int), null, CultureInfo.CurrentCulture);
        }
    }
}
