using System.ComponentModel;
using GeoGen.Studio.PlugInLoader;

namespace GeoGen.Studio.Utilities.PlugInBase
{
    /// <summary>
    /// Base class for plug-ins that are not based on any class (other than <see cref="object"/>). Provides <see cref="INotifyPropertyChanged"/> implementation.
    /// </summary>
    public class Object : IPlugIn, INotifyPropertyChanged
    {
        /// <summary>
        /// Occurs when a property value changes.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        protected void OnPropertyChanged(string info)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(info));
            }
        }
    }
}
