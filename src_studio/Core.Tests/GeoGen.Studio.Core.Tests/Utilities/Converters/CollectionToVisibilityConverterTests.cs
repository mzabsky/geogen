namespace GeoGen.Studio.Utilities.Converters
{
	using System.Collections.Generic;
	using System.Globalization;
	using System.Linq;
	using System.Windows;
	using System.Windows.Data;

	using NUnit.Framework;

	// ReSharper disable InconsistentNaming

	/// <summary>
	/// Tests for <see cref="CollectionToVisibilityConverter"/> class.
	/// </summary>
	[TestFixture]
	public class CollectionToVisibilityConverterTests
	{
		/// <summary>
		/// Tests that instance getter really returns valids instance.
		/// </summary>
		[Test]
		public void Instance_ReturnsInstance()
		{
			Assert.IsInstanceOf<CollectionToVisibilityConverter>(CollectionToVisibilityConverter.Instance);
		}

		/// <summary>
		/// Tests that collection gets converted to <see cref="Visibility.Visible"/>.
		/// </summary>
		[Test]
		public void Convert_Collection_ReturnsVisible()
		{
			IValueConverter converter = new CollectionToVisibilityConverter();
			Visibility result = (Visibility)converter.Convert(new List<int> { 2, 4, 6 }, typeof(Visibility), null, CultureInfo.CurrentCulture);

			Assert.AreEqual(result, Visibility.Visible);
		}

		/// <summary>
		/// Tests that empty collection gets converted to <see cref="Visibility.Collapsed"/>.
		/// </summary>
		[Test]
		public void Convert_Empty_ReturnsCollapsed()
		{
			IValueConverter converter = new CollectionToVisibilityConverter();
			Visibility result = (Visibility)converter.Convert(Enumerable.Empty<int>(), typeof(Visibility), null, CultureInfo.CurrentCulture);

			Assert.AreEqual(result, Visibility.Collapsed);
		}

		/// <summary>
		/// Tests that anything else gets converted to "Unknown value".
		/// </summary>
		[Test]
		public void Convert_Invalid_ReturnsInvalid()
		{
			IValueConverter converter = new CollectionToVisibilityConverter();
			string result = (string)converter.Convert(new object(), typeof(Visibility), null, CultureInfo.CurrentCulture);

			Assert.AreEqual(result, "<Unknown value>");
		}

		/// <summary>
		/// Tests that ConverBack doesn't crash.
		/// </summary>
		[Test]
		public void ConvertBack_Null_NoException()
		{
			IValueConverter converter = new CollectionToVisibilityConverter();
			converter.ConvertBack(null, typeof(int), null, CultureInfo.CurrentCulture);
		}
	}
}
