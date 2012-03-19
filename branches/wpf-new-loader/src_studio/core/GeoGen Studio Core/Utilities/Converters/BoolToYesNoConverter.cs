namespace GeoGen.Studio.Utilities.Converters
{
	using System;
	using System.Globalization;
	using System.Windows.Data;

	/// <summary>
	/// Converts a boolean value to a "Yes"/"No" string value.
	/// </summary>
	[ValueConversion(typeof(bool), typeof(String))]
	public sealed class BoolToYesNoConverter : IValueConverter
	{
		private static readonly IValueConverter instance = new BoolToYesNoConverter();

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
				if ((bool)value) return "Yes";

				return "No";
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
