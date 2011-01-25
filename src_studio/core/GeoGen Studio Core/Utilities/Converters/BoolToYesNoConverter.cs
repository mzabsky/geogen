using System;
using System.Globalization;
using System.Windows.Data;

namespace GeoGen.Studio.Utilities.Converters
{
    /// <summary>
    /// Converts a boolean value to a "Yes"/"No" string value.
    /// </summary>
    [ValueConversion(typeof(bool), typeof(String))]
    public class BoolToYesNoConverter: IValueConverter
    {
        object IValueConverter.Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            try
            {
                if ((bool) value) return "Yes";
                
                return "No";
            }
            catch(InvalidCastException) { }
            return "<Unknown value>";
        }

        object IValueConverter.ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return null;
        }
    }
}
