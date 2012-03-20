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
	/// Tests for <see cref="StringToVisibilityConverter"/> class.
	/// </summary>
	[TestFixture]
	public class StringToVisibilityConverterTests
	{
		/// <summary>
		/// Tests that instance getter really returns valids instance.
		/// </summary>
		[Test]
		public void Instance_ReturnsInstance()
		{
			Assert.IsInstanceOf<StringToVisibilityConverter>(StringToVisibilityConverter.Instance);
		}

		/// <summary>
		/// Tests that string gets converted to <see cref="Visibility.Visible"/>.
		/// </summary>
		[Test]
		public void Convert_String_ReturnsVisible()
		{
			IValueConverter converter = new StringToVisibilityConverter();
			Visibility result = (Visibility)converter.Convert("foo", typeof(Visibility), null, CultureInfo.CurrentCulture);

			Assert.AreEqual(result, Visibility.Visible);
		}

		/// <summary>
		/// Tests that empty string gets converted to <see cref="Visibility.Collapsed"/>.
		/// </summary>
		[Test]
		public void Convert_Empty_ReturnsCollapsed()
		{
			IValueConverter converter = new StringToVisibilityConverter();
			Visibility result = (Visibility)converter.Convert("", typeof(Visibility), null, CultureInfo.CurrentCulture);

			Assert.AreEqual(result, Visibility.Collapsed);
		}

		/// <summary>
		/// Tests that null gets converted to <see cref="Visibility.Collapsed"/>.
		/// </summary>
		[Test]
		public void Convert_Null_ReturnsCollapsed()
		{
			IValueConverter converter = new StringToVisibilityConverter();
			Visibility result = (Visibility)converter.Convert(null, typeof(Visibility), null, CultureInfo.CurrentCulture);

			Assert.AreEqual(result, Visibility.Collapsed);
		}

		/// <summary>
		/// Tests that anything else gets converted to "Unknown value".
		/// </summary>
		[Test]
		public void Convert_Invalid_ReturnsInvalid()
		{
			IValueConverter converter = new StringToVisibilityConverter();
			string result = (string)converter.Convert(new object(), typeof(Visibility), null, CultureInfo.CurrentCulture);

			Assert.AreEqual(result, "<Unknown value>");
		}

		/// <summary>
		/// Tests that ConverBack doesn't crash.
		/// </summary>
		[Test]
		public void ConvertBack_Null_NoException()
		{
			IValueConverter converter = new StringToVisibilityConverter();
			converter.ConvertBack(null, typeof(int), null, CultureInfo.CurrentCulture);
		}
	}
}
