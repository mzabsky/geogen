namespace GeoGen.Studio.Utilities.Converters
{
    using System;
    using System.Globalization;
    using System.Windows.Data;

    /// <summary>
    /// Converts full file path to a file name.
    /// </summary>
    [ValueConversion(typeof(bool), typeof(String))]
    public sealed class FullPathToFileNameConverter : IValueConverter
    {
        /// <summary>
        /// Backing field for <see cref="Instance"/>.
        /// </summary>
        private static readonly IValueConverter instance = new FullPathToFileNameConverter();

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

        /// <inheritdoc />
        object IValueConverter.Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value != null)
            {
                try
                {
                    var stringValue = (string)value;

                    var lastSlashPosition = stringValue.LastIndexOfAny("\\/".ToCharArray());
                    if (lastSlashPosition == -1)
                    {
                        return stringValue;
                    }

                    return stringValue.Substring(lastSlashPosition + 1);
                }
                catch (InvalidCastException)
                {
                }
            }

            return "<Unknown value>";
        }

        /// <inheritdoc />
        object IValueConverter.ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return null;
        }
    }
}
