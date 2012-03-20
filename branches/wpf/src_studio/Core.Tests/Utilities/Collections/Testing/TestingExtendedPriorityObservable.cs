namespace GeoGen.Studio.Utilities.Collections.Testing
{
    /// <summary>
    /// Adds one more property to test that <see cref="PriorityObservableCollection{T}"/> does not react on non-priority properties.
    /// </summary>
    class TestingExtendedPriorityObservable : TestingPriorityObservable
    {
        /// <summary>
        /// Property which is not Priority.
        /// </summary>
        /// <value>
        /// Irrelevant value.
        /// </value>
        public double NotPriority
        {
            set
            {
                this.OnPropertyChanged("NotPriority");
            }
        }
    }
}
