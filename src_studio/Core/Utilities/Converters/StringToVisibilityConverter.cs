namespace GeoGen.Studio.Utilities.Converters
{
    using System;
    using System.Globalization;
    using System.Windows;
    using System.Windows.Data;

    /// <summary>
    /// Converts a string to <see cref="Visibility"/>. Empty string (or null) will be converted to value Collapsed, any other string will be converted to Visible.
    /// </summary>
    [ValueConversion(typeof(string), typeof(Visibility))]
    public sealed class StringToVisibilityConverter : IValueConverter
    {
        private static readonly IValueConverter instance = new StringToVisibilityConverter();

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
                if (string.IsNullOrEmpty((string)value)) return Visibility.Collapsed;

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
