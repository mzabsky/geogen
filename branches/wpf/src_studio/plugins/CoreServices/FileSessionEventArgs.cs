namespace GeoGen.Studio.PlugIns.Services
{
    using System;

    /// <summary>
    /// Provides data for <see cref="IFileSession"/> events.
    /// </summary>
    public class FileSessionEventArgs : EventArgs
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="FileSessionEventArgs"/> class.
        /// </summary>
        /// <param name="origin">The origin.</param>
        public FileSessionEventArgs(object origin)
        {
            this.Origin = origin;
        }
        
        /// <summary>
        /// Gets the object which triggered the <see cref="IFileSession"/> event.
        /// </summary>
        /// <value>
        /// The origin.
        /// </value>
        public object Origin { get; private set; }
    }
}
