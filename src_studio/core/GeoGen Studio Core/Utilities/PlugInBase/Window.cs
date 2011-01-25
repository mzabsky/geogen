using System.ComponentModel;
using GeoGen.Studio.PlugInLoader;

namespace GeoGen.Studio.Utilities.PlugInBase
{
    /// <summary>
    /// Base class for plug-ins extending WPF <see cref="System.Windows.Window"/>. Provides <see cref="INotifyPropertyChanged"/> implementation.
    /// </summary>
    public class Window : System.Windows.Window, IPlugIn, INotifyPropertyChanged
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
