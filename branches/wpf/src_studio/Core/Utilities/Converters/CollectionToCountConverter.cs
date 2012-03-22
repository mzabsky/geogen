namespace GeoGen.Studio.Utilities.Converters
{
    using System;
    using System.Collections;
    using System.Globalization;
    using System.Linq;
    using System.Windows.Data;

    /// <summary>
    /// Converts a collection to an integer representing count of its items.
    /// </summary>
    [ValueConversion(typeof(IEnumerable), typeof(int))]
    public sealed class CollectionToCountConverter : IValueConverter
    {
        private static readonly IValueConverter instance = new CollectionToCountConverter();

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
            if (value == null || value as IEnumerable == null)
            {
                return -1;
            }

            int count = (value as IEnumerable).Cast<object>().Count();

            return count;
        }

        object IValueConverter.ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return null;
        }
    }
}
