namespace GeoGen.Studio.PlugIns.Services
{
    using System;

    /// <summary>
    /// The exception that is thrown when a method is called on already closed session.
    /// </summary>
    public class SessionClosedException : InvalidOperationException
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="SessionClosedException"/> class.
        /// </summary>
        public SessionClosedException() : base("The session is already closed.")
        {            
        }
    }
}
