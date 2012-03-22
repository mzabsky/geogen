namespace GeoGen.Studio.Utilities.Converters
{
    using System;
    using System.Globalization;
    using System.Windows.Data;
    using System.Windows.Media.Imaging;
    using GeoGen.Studio;
    using System.Windows;

    /// <summary>
    /// Converts <see cref="HeightData"/> and <see cref="Overlay"/> to <see cref="BitmapSource"/>. If <see cref="Overlay"/> is null, a simple grayscale representation of the height map is returned.
    /// </summary>
    public class HeightDataAndOverlayToBitmapConverter : IMultiValueConverter
    {
        private static readonly IMultiValueConverter instance = new HeightDataAndOverlayToBitmapConverter();
        /// <summary>
        /// Gets instance of this converter.
        /// </summary>
        /// <value>The instance.</value>
        public static IMultiValueConverter Instance
        {
            get
            {
                return instance;
            }
        }

        object IMultiValueConverter.Convert(object[] value, Type targetType, object parameter, CultureInfo culture)
        {
            HeightData heightData = (HeightData) value[0];

            Overlay overlay = null;
            if(value.Length > 1 && value[1] != DependencyProperty.UnsetValue){
                overlay = (Overlay) value[1];
            }

            if (heightData == null)
            {
                //throw new ArgumentNullException("height data");
                return null;
            }
            
            if (overlay == null)
            {
                return heightData.GetBaseImage();
            }
            else
            {
                return heightData.GetImage(overlay);
            }
        }

        object[] IMultiValueConverter.ConvertBack(object value, Type[] targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}


