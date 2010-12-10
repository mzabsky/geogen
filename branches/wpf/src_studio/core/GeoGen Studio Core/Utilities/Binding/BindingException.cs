using System;

namespace GeoGen.Studio.Utilities.Binding
{
    /// <summary>
    /// Represents an exception thrown during property synchronization.
    /// </summary>
    public class BindingException: Exception
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="BindingException"/> class.
        /// </summary>
        /// <param name="message">The message.</param>
        public BindingException(string message): base(message) {}

        /// <summary>
        /// Initializes a new instance of the <see cref="BindingException"/> class.
        /// </summary>
        /// <param name="message">The message.</param>
        /// <param name="innerException">The inner exception.</param>
        public BindingException(string message, Exception innerException) : base(message, innerException) { }
    }
}
