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
	/// Tests for <see cref="ObjectToVisibilityConverter"/> class.
	/// </summary>
	[TestFixture]
	public class ObjectToVisibilityConverterTests
	{
		/// <summary>
		/// Tests that instance getter really returns valids instance.
		/// </summary>
		[Test]
		public void TestInstance_ReturnsInstance()
		{
			Assert.IsInstanceOf<ObjectToVisibilityConverter>(ObjectToVisibilityConverter.Instance);
		}

		/// <summary>
		/// Tests that object gets converted to <see cref="Visibility.Visible"/>.
		/// </summary>
		[Test]
		public void TestConvert_Object_ReturnsVisible()
		{
			IValueConverter converter = new ObjectToVisibilityConverter();
			Visibility result = (Visibility)converter.Convert(new object(), typeof(Visibility), null, CultureInfo.CurrentCulture);

			Assert.AreEqual(result, Visibility.Visible);
		}

		/// <summary>
		/// Tests that <c>null</c> collection gets converted to <see cref="Visibility.Collapsed"/>.
		/// </summary>
		[Test]
		public void TestConvert_Null_ReturnsCollapsed()
		{
			IValueConverter converter = new ObjectToVisibilityConverter();
			Visibility result = (Visibility)converter.Convert(null, typeof(Visibility), null, CultureInfo.CurrentCulture);

			Assert.AreEqual(result, Visibility.Collapsed);
		}

		/// <summary>
		/// Tests that ConverBack doesn't crash.
		/// </summary>
		[Test]
		public void TestConvertBack_Null_NoException()
		{
			IValueConverter converter = new ObjectToVisibilityConverter();
			converter.ConvertBack(null, typeof(int), null, CultureInfo.CurrentCulture);
		}
	}
}
