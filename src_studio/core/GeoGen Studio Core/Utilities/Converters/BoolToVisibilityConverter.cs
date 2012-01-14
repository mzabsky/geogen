namespace GeoGen.Studio.Utilities.Converters
{
	using System;
	using System.Globalization;
	using System.Windows;
	using System.Windows.Data;

	/// <summary>
	/// Converts a bool to <see cref="Visibility"/>. False (or null) will be converted to value Collapsed, any true will be converted to Visible.
	/// </summary>
	[ValueConversion(typeof(bool), typeof(Visibility))]
	public sealed class BoolToVisibilityConverter : IValueConverter
	{
		private static readonly IValueConverter instance = new BoolToVisibilityConverter();

		/// <summary>
		/// Gets instance of this converter.
		/// </summary>
		/// <value>The instance.</value>
		public static IValueConverter Instance
		{
			get
			{
				return instance;
			}
		}

		object IValueConverter.Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			try
			{
				if (value == null || !((bool)value)) return Visibility.Collapsed;

				return Visibility.Visible;
			}
			catch (InvalidCastException) { }
			return "<Unknown value>";
		}

		object IValueConverter.ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			return null;
		}
	}
}

