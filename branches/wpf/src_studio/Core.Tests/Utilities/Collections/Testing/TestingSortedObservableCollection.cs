namespace GeoGen.Studio.Utilities.Collections.Testing
{
    using System;
    using System.ComponentModel;

    /// <summary>
    /// Concrete implementation of <see cref="SortedObservableCollection{TValue}"/> for testing.
    /// </summary>
    public class TestingSortedObservableCollection : SortedObservableCollection<TestingPriorityObservable>
    {
        private readonly Action<string> itemChangedCallback;

        /// <summary>
        /// Initializes a new instance of the <see cref="TestingSortedObservableCollection"/> class.
        /// </summary>
        public TestingSortedObservableCollection()
        {
            this.Comparer = new PriorityComparer();
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="TestingSortedObservableCollection"/> class.
        /// </summary>
        /// <param name="itemChangedCallback">The item changed callback.</param>
        public TestingSortedObservableCollection(Action<string> itemChangedCallback) :
            base(new PriorityComparer())
        {
            this.itemChangedCallback = itemChangedCallback;
        }

        /// <summary>
        /// Invoked by the superclass when item changes - triggers the item changed callback.
        /// </summary>
        /// <param name="o">Sender.</param>
        /// <param name="args">The <see cref="System.ComponentModel.PropertyChangedEventArgs"/> instance containing the event data.</param>
        protected override void ItemPropertyChanged(object o, PropertyChangedEventArgs args)
        {
            if (itemChangedCallback != null)
            {
                itemChangedCallback(args.PropertyName);
            }
        }
    }
}
