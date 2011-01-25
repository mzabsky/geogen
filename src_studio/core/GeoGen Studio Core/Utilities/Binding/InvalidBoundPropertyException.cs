namespace GeoGen.Studio.Utilities.Binding
{
    /// <summary>
    /// Represents an exception thrown when either of bound properties is accessible or the properties' type doesn't match.
    /// </summary>
    public class InvalidBoundPropertyException: BindingException
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="InvalidBoundPropertyException"/> class.
        /// </summary>
        /// <param name="message">The message.</param>
        public InvalidBoundPropertyException(string message): base(message) {}
    }
}
