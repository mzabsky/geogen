namespace GeoGen.Studio.Utilities.Converters
{
	using System.Globalization;
	using System.Windows;
	using System.Windows.Data;

	using NUnit.Framework;

	// ReSharper disable InconsistentNaming

	/// <summary>
	/// Tests for <see cref="BoolToVisibilityConverter"/> class.
	/// </summary>
	[TestFixture]
	public class BoolToVisibilityConverterTests
	{
		/// <summary>
		/// Tests that instance getter really returns valids instance.
		/// </summary>
		[Test]
		public void Instance_ReturnsInstance()
		{
			Assert.IsInstanceOf<BoolToVisibilityConverter>(BoolToVisibilityConverter.Instance);
		}

		/// <summary>
		/// Tests that <c>false</c> gets converted to <see cref="Visibility.Visible"/>.
		/// </summary>
		[Test]
		public void Convert_True_ReturnsVisible()
		{
			IValueConverter converter = new BoolToVisibilityConverter();
			Visibility result = (Visibility)converter.Convert(true, typeof(Visibility), null, CultureInfo.CurrentCulture);

			Assert.AreEqual(result, Visibility.Visible);
		}

		/// <summary>
		/// Tests that <c>false</c> gets converted to <see cref="Visibility.Collapsed"/>.
		/// </summary>
		[Test]
		public void Convert_False_ReturnsCollapsed()
		{
			IValueConverter converter = new BoolToVisibilityConverter();
			Visibility result = (Visibility)converter.Convert(false, typeof(Visibility), null, CultureInfo.CurrentCulture);

			Assert.AreEqual(result, Visibility.Collapsed);
		}

		/// <summary>
		/// Tests that anything else gets converted to "Unknown value".
		/// </summary>
		[Test]
		public void Convert_Invalid_ReturnsInvalid()
		{
			IValueConverter converter = new BoolToVisibilityConverter();
			string result = (string)converter.Convert(new object(), typeof(Visibility), null, CultureInfo.CurrentCulture);

			Assert.AreEqual(result, "<Unknown value>");
		}

		/// <summary>
		/// Tests that ConverBack doesn't crash.
		/// </summary>
		[Test]
		public void ConvertBack_Null_NoException()
		{
			IValueConverter converter = new BoolToVisibilityConverter();
			converter.ConvertBack(null, typeof(int), null, CultureInfo.CurrentCulture);
		}
	}
}
