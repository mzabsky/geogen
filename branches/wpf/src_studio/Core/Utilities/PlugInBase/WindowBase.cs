using System.ComponentModel;
using GeoGen.Studio.PlugInLoader;
using GeoGen.Studio.PlugIns.Interfaces;

namespace GeoGen.Studio.Utilities.PlugInBase
{
    /// <summary>
    /// Base class for plug-ins extending WPF <see cref="System.Windows.Window"/>. Provides <see cref="INotifyPropertyChanged"/>, <see cref="IControl"/> and <see cref="IWindow"/> implementation.
    /// </summary>
    public class WindowBase : System.Windows.Window, IPlugIn, INotifyPropertyChanged, IControl, IWindow
    {
        /// <summary>
        /// Occurs when a property value changes.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        System.Windows.Controls.Control IControl.Control
        {
            get
            {
                return this;
            }
        }

        public virtual  System.Windows.Window Window
        {
            get
            {
                return this;
            }
        }

        protected virtual void OnPropertyChanged(string info)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(info));
            }
        }
    }
}
