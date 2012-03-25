namespace GeoGen.Studio.PlugIns
{
    using System;
    using System.Globalization;
    using System.Windows;
    using System.Windows.Data;

    /// <summary>
    /// Converts a bool to <see cref="Visibility"/>. False (or null) will be converted to value Collapsed, any true will be converted to Visible.
    /// </summary>
    [ValueConversion(typeof(bool), typeof(Visibility))]
    public sealed class ObjectToEnumValuesConverter : IValueConverter
    {
        /// <summary>
        /// Backing field for <see cref="Instance"/>.
        /// </summary>
        private static readonly IValueConverter instance = new ObjectToEnumValuesConverter();

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

        /// <summary>
        /// Converts a value.
        /// </summary>
        /// <param name="value">The value produced by the binding source.</param>
        /// <param name="targetType">The type of the binding target property.</param>
        /// <param name="parameter">The converter parameter to use.</param>
        /// <param name="culture">The culture to use in the converter.</param>
        /// <returns>
        /// A converted value. If the method returns null, the valid null value is used.
        /// </returns>
        object IValueConverter.Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value == null || !value.GetType().IsEnum)
            {
                return new[] { "<Not Enum>" };
            }

            return value.GetType().GetEnumValues();
        }

        /// <summary>
        /// Converts a value.
        /// </summary>
        /// <param name="value">The value that is produced by the binding target.</param>
        /// <param name="targetType">The type to convert to.</param>
        /// <param name="parameter">The converter parameter to use.</param>
        /// <param name="culture">The culture to use in the converter.</param>
        /// <returns>
        /// A converted value. If the method returns null, the valid null value is used.
        /// </returns>
        object IValueConverter.ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return null;
        }
    }
}
