namespace GeoGen.Studio.Utilities.Collections
{
    using System;

    /// <summary>
    /// The exception caused by a changing operation being called on a read-only exception.
    /// </summary>
    public class CollectionReadOnlyException : InvalidOperationException
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="CollectionReadOnlyException"/> class.
        /// </summary>
        public CollectionReadOnlyException()
            : base("This collection is read-only.")
        {
        }
    }
}
