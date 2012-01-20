namespace GeoGen.Studio.Utilities.Collections.Testing
{
    using System.ComponentModel;

    /// <summary>
    /// <see cref="ObservableObject"/> with single property for observable collection testing purposes.
    /// </summary>
    public class TestingPriorityObservable : ObservableObject, IPriority, INotifyPropertyChanged
    {
        private double priority;

        /// <summary>
        /// Property used to test <see cref="INotifyPropertyChanged"/>.
        /// </summary>
        /// <value>
        /// The value.
        /// </value>
        public double Priority {
            get
            {
                return this.priority;
            }

            set
            {
                this.priority = value;
                this.OnPropertyChanged("Priority");
            }
        }
    }
}
