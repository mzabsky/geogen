namespace GeoGen.Studio.Utilities.Converters
{
    using System;
    using System.Globalization;
    using System.Windows;
    using System.Windows.Data;

    /// <summary>
    /// Converts an <see cref="object"/> to <see cref="Visibility"/>. Null will be converted to value Collapsed, any other string will be converted to Visible.
    /// </summary>
    [ValueConversion(typeof(object)
, typeof(Visibility))]
    public sealed class ObjectToVisibilityConverter : IValueConverter
    {
        private static readonly IValueConverter instance = new ObjectToVisibilityConverter();

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
            if (value == null) return Visibility.Collapsed;

            return Visibility.Visible;
        }

        object IValueConverter.ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return null;
        }
    }
}
