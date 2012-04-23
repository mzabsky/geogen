namespace GeoGen.Studio.Utilities.PlugInBase
{
    using System.ComponentModel;
    using System.Windows.Controls;
    using GeoGen.Studio.PlugInLoader;
    using GeoGen.Studio.PlugIns.Interfaces;
    
    
    /// <summary>
    /// Base class for plug-ins extending WPF <see cref="UserControl"/>. Provides <see cref="INotifyPropertyChanged"/> and <see cref="IControl"/> implementation.
    /// </summary>
    public class ControlBase : UserControl, IPlugIn, IControl
    {
        /// <summary>
        /// Occurs when a property value changes.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        public virtual System.Windows.Controls.Control Control
        {
            get {
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
