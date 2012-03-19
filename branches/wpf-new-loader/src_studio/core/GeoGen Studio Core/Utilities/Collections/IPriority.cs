namespace GeoGen.Studio.Utilities.Collections
{
    /// <summary>
    /// Object that can be sorted by priority within an collection.
    /// </summary>
    public interface IPriority
    {
        /// <summary>
        /// The priority.
        /// </summary>
        /// <value>The priority.</value>
        double Priority {get;}
    }
}
