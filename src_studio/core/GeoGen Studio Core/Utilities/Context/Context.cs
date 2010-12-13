namespace GeoGen.Studio.Utilities.Context
{
    /// <summary>
    /// Represents an application-wide context.
    /// </summary>
    public sealed class Context
    {
        /// <summary>
        /// Label of the <see cref="Context"/>. Two <see cref="Context">contexts</see> are not considered equal only because they have the same label (reference comparison is performed).
        /// </summary>
        /// <value>The label.</value>
        public string Label { get; private set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="Context"/> class.
        /// </summary>
        /// <param name="text">The label.</param>
        public Context(string label)
        {
            this.Label = label;
        }
    }
}
