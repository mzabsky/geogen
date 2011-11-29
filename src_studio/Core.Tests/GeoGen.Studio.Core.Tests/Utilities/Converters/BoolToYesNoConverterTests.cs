using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;
using System.Windows.Data;
using System.Globalization;

namespace GeoGen.Studio.Utilities.Converters
{
    [TestFixture]
    public class BoolToYesNoConverterTests
    {
        [Test]
        public void TestInstance_ReturnsInstance()
        {
            Assert.IsInstanceOf<BoolToYesNoConverter>(BoolToYesNoConverter.Instance);
        }
        
        [Test]
        public void TestConvert_true_ReturnsYes()
        {
            IValueConverter converter = new BoolToYesNoConverter();
            string result = (string) converter.Convert(true, typeof(string), null, CultureInfo.CurrentCulture);

            Assert.AreEqual(result, "Yes");
        }

        [Test]
        public void TestConvert_false_ReturnsNo()
        {
            IValueConverter converter = new BoolToYesNoConverter();
            string result = (string)converter.Convert(false, typeof(string), null, CultureInfo.CurrentCulture);

            Assert.AreEqual(result, "No");
        }
    }
}
