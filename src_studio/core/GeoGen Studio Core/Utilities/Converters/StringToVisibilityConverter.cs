using System;
using System.Globalization;
using System.Windows;
using System.Windows.Data;

namespace GeoGen.Studio.Utilities.Converters
{
    /// <summary>
    /// Converts a string to <see cref="Visibility"/>. Empty string (or null) will be converted to value Collapsed, any other string will be converted to Visible.
    /// </summary>
    [ValueConversion(typeof(string), typeof(Visibility))]
    public class StringToVisibilityConverter : IValueConverter
    {
        object IValueConverter.Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            try
            {
                if (value == null || (string)value == "") return Visibility.Collapsed;
                
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
