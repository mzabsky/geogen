namespace GeoGen.Studio.PlugIns.Services
{
    using System;

    /// <summary>
    /// Contains implementation of <see cref="IFileSession"/>.
    /// </summary>
    public partial class FileService
    {
        /// <summary>
        /// Represents one file managed by <see cref="FileService"/>.
        /// </summary>
        private class FileSession : IFileSession
        {
            /// <summary>
            /// The <see cref="FileService"/> which created this instance.
            /// </summary>
            private readonly FileService service;

            /// <summary>
            /// File system path to which this session is associated.
            /// </summary>
            private readonly string fileName;

            /// <summary>
            /// True if theere is not filesystem file corresponding to this session.
            /// </summary>
            private readonly bool isBound;

            /// <summary>
            /// Initializes a new instance of the <see cref="FileSession"/> class.
            /// </summary>
            /// <param name="service">The service.</param>
            /// <param name="fileName">Name of the file.</param>
            internal FileSession(FileService service, string fileName)
            {
                if (fileName == null)
                {
                    throw new ArgumentNullException("fileName");
                }

                if (fileName == string.Empty)
                {
                    throw new ArgumentException("FileName must not be empty.");
                }
                
                this.service = service;
                this.fileName = fileName;
                this.IsClosed = false;
                this.IsUnsaved = false;

                // Unbound file names are in format "*.extension"
                this.isBound = fileName[0] != '*';
            }

            /// <inheritdoc/>
            public event EventHandler<FileSessionEventArgs> Changed;

            /// <inheritdoc/>
            public event EventHandler<FileSessionEventArgs> Closed;

            /// <inheritdoc/>
            public event EventHandler<FileSessionEventArgs> Saved;

            /// <inheritdoc/>
            public string FileName
            {
                get
                {
                    return this.fileName;
                }
            }

            /// <inheritdoc/>
            public string Extension
            {
                get
                {
                    var lastDotPosition = this.FileName.LastIndexOf('.');

                    if (lastDotPosition == -1)
                    {
                        return string.Empty;
                    }

                    return this.FileName.Substring(lastDotPosition);
                }
            }

            /// <inheritdoc/>
            public bool IsUnsaved { get; private set; }

            /// <inheritdoc/>
            public bool IsClosed { get; private set; }

            /// <inheritdoc/>
            public bool IsBound
            {
                get
                {
                    return this.isBound;
                }
            }

            /// <inheritdoc/>
            public void OnChanged(object origin)
            {
                if (origin == null)
                {
                    throw new ArgumentNullException("origin");
                }

                if (this.IsClosed)
                {
                    throw new SessionClosedException();
                }

                this.IsUnsaved = true;

                if (this.Changed != null)
                {
                    var args = new FileSessionEventArgs(origin);
                    this.Changed(this, args);
                }
            }

            /// <inheritdoc/>
            public void OnClosed(object origin)
            {
                if (origin == null)
                {
                    throw new ArgumentNullException("origin");
                }

                if (this.IsClosed)
                {
                    throw new SessionClosedException();
                }

                this.IsClosed = true;

                this.service.activeSessions.Remove(this);

                if (this.Closed != null)
                {
                    var args = new FileSessionEventArgs(origin);
                    this.Closed(this, args);
                }
            }

            /// <inheritdoc/>
            public void OnSaved(object origin)
            {
                if (origin == null)
                {
                    throw new ArgumentNullException("origin");
                }

                if (this.IsClosed)
                {
                    throw new SessionClosedException();
                }

                this.IsUnsaved = false;

                if (this.Saved != null)
                {
                    var args = new FileSessionEventArgs(origin);
                    this.Saved(this, args);
                }
            }

            /// <inheritdoc/>
            public IFileSession OnSavedAs(object origin, string fileName)
            {
                if (origin == null)
                {
                    throw new ArgumentNullException("origin");
                }

                if (fileName == null)
                {
                    throw new ArgumentNullException("fileName");
                }

                if (this.IsClosed)
                {
                    throw new SessionClosedException();
                }

                this.OnClosed(origin);

                var newSession = this.service.OnOpened(origin, fileName);

                newSession.OnSaved(origin);

                return newSession;
            }
        }  
    }  
}
