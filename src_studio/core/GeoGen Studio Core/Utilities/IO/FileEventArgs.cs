using System;

namespace GeoGen.Studio.Utilities.IO
{
    /// <summary>
    /// Signature for file service event delegate.
    /// </summary>
    /// <param name="sender">The sender.</param>
    /// <param name="args">The <see cref="FileEventArgs"/> instance containing the event data.</param>
    public delegate void FileEventHandler (object sender, FileEventArgs args);

    /// <summary>
    /// Event data for file service events.
    /// </summary>
    public class FileEventArgs: EventArgs
    {
        /// <summary>
        /// Info of the file being manipulated.
        /// </summary>
        public FileSession FileSession { get; private set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="FileEventArgs"/> class.
        /// </summary>
        /// <param name="fileSession">The file session.</param>
        public FileEventArgs(FileSession fileSession)
        {
            this.FileSession = fileSession;
        }
    }
}
