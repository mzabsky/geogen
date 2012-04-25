namespace GeoGen.Studio.Utilities.Collections
{
    using System;

    /// <summary>
    /// <see cref="PriorityObservableCollection{T}"/> which adds a separator between adjacent items which don't share <see cref="IPriorityGroupable.Group"/>.
    /// </summary>
    /// <typeparam name="TValue">Value type.</typeparam>
    public class GroupedPriorityObservableCollection<TValue> : PriorityObservableCollection<TValue> where TValue : IPriorityGroupable
    {
        /// <summary>
        /// Instances of this type will be used as separators between groups.
        /// </summary>
        private readonly Type groupSeparatorType;

        /// <summary>
        /// Initializes a new instance of the <see cref="GroupedPriorityObservableCollection&lt;TValue&gt;"/> class.
        /// </summary>
        /// <param name="groupSeparatorType">Type of the group separator.</param>
        public GroupedPriorityObservableCollection(Type groupSeparatorType)
        {
            this.groupSeparatorType = groupSeparatorType;
        }

        /// <inheritdoc />
        public override void Insert(int index, TValue value)
        {
            base.Insert(index, value);

            if (value.GetType() == this.groupSeparatorType)
            {
                return;
            }
            
            // Test if the item has the same group as the previous item
            if (index != 0)
            {
                if (this[index - 1].Group != value.Group && this[index - 1].GetType() != this.groupSeparatorType)
                {
                    this.Insert(index, this.GetGroupSeparator());

                    index++;
                }
            }

            // Test if the item has the same group as the next item
            if (index != this.Count - 1)
            {
                if (this[index + 1].Group != value.Group && this[index + 1].GetType() != this.groupSeparatorType)
                {
                    this.Insert(index + 1, this.GetGroupSeparator());
                }
            }
        }

        /// <summary>
        /// Gets a new instance of the group separator.
        /// </summary>
        /// <returns>The group separator instance.</returns>
        protected virtual TValue GetGroupSeparator()
        {
            return (TValue)Activator.CreateInstance(this.groupSeparatorType);
        }
    }
}
