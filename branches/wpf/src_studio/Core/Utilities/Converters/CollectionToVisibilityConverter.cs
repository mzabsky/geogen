namespace GeoGen.Studio.Utilities.Converters
{
    using System;
    using System.Collections;
    using System.Globalization;
    using System.Linq;
    using System.Windows;
    using System.Windows.Data;

    /// <summary>
    /// Converts a collection to <see cref="Visibility"/>. Empty collection (or null) will be converted to value Collapsed, any non-empty collection will be converted to Visible.
    /// </summary>
    [ValueConversion(typeof(IEnumerable), typeof(Visibility))]
    public sealed class CollectionToVisibilityConverter : IValueConverter
    {
        private static readonly IValueConverter instance = new CollectionToVisibilityConverter();

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
                if (value == null || !((IEnumerable)value).Cast<object>().Any()) return Visibility.Collapsed;

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

