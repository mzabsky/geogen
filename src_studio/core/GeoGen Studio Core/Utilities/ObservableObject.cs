using System.ComponentModel;

namespace GeoGen.Studio.Utilities
{
    /// <summary>
    /// Provides <see cref="INotifyPropertyChanged"/> implementation.
    /// </summary>
    public class ObservableObject: INotifyPropertyChanged
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
