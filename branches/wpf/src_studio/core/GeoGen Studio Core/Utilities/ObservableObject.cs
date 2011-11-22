namespace GeoGen.Studio.Utilities
{
    using System.ComponentModel;

    /// <summary>
    /// Provides <see cref="INotifyPropertyChanged"/> implementation.
    /// </summary>
    public abstract class ObservableObject
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
