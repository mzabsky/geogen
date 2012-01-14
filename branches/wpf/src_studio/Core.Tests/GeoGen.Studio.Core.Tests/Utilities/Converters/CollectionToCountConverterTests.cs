namespace GeoGen.Studio.Utilities.Converters
{
	using System.Collections.Generic;
	using System.Globalization;
	using System.Windows.Data;
	using NUnit.Framework;

	// ReSharper disable InconsistentNaming
	/// <summary>
	/// Tests for <see cref="CollectionToCountConverter"/> class.
	/// </summary>
	[TestFixture]
	public class CollectionToCountConverterTests
	{
		/// <summary>
		/// Tests that instance getter really returns valids instance.
		/// </summary>
		[Test]
		public void TestInstance_ReturnsInstance()
		{
			Assert.IsInstanceOf<CollectionToCountConverter>(CollectionToCountConverter.Instance);
		}

		/// <summary>
		/// Tests that a collection gets converted to its count.
		/// </summary>
		[Test]
		public void TestConvert_Collection_ReturnsCount()
		{
			IValueConverter converter = new CollectionToCountConverter();
			int result = (int)converter.Convert(new List<int> { 2, 4, 6 }, typeof(int), null, CultureInfo.CurrentCulture);

			Assert.AreEqual(result, 3);
		}

		/// <summary>
		/// Tests that a non-collection gets converted to -1.
		/// </summary>
		[Test]
		public void TestConvert_Invalid_ReturnsMinusOne()
		{
			IValueConverter converter = new CollectionToCountConverter();
			int result = (int)converter.Convert(null, typeof(int), null, CultureInfo.CurrentCulture);

			Assert.AreEqual(result, -1);
		}

		/// <summary>
		/// Tests that ConverBack doesn't crash.
		/// </summary>
		[Test]
		public void TestConvertBack_Null_NoException()
		{
			IValueConverter converter = new CollectionToCountConverter();
			converter.ConvertBack(null, typeof(int), null, CultureInfo.CurrentCulture);
		}
	}
}
