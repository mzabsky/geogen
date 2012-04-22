namespace GeoGen.Studio.PlugIns.Services
{
    using System;
    using System.Collections.ObjectModel;

    using GeoGen.Studio.Utilities.Collections;
    using GeoGen.Studio.Utilities.Persistence;
    using GeoGen.Studio.Utilities.PlugInBase;

    // TODO: Persistence of RecentFiles.

    /// <summary>
    /// Manages files opened by various plug-ins.
    /// </summary>
    public partial class FileService : ObjectBase, IFileService
    {
        /// <summary>
        /// Backing field for <see cref="ActiveSessions"/>.
        /// </summary>
        private readonly ObservableCollection<IFileSession> activeSessions = new ObservableCollection<IFileSession>();

        /// <summary>
        /// Read-only view for <see cref="ActiveSessions"/>.
        /// </summary>
        private readonly ReadOnlyObservableCollection<IFileSession> activeSessionsView;

        /// <summary>
        /// Backing field for <see cref="RecentFiles"/>.
        /// </summary>
        private UniqueObservableQueue<string> recentFiles = new UniqueObservableQueue<string>();

        /// <summary>
        /// Read-only view for for <see cref="RecentFiles"/>.
        /// </summary>
        private ReadOnlyObservableCollectionView<string> recentFilesView;

        /// <summary>
        /// Initializes a new instance of the <see cref="FileService"/> class.
        /// </summary>
        public FileService()
        {
            this.activeSessionsView = new ReadOnlyObservableCollection<IFileSession>(this.activeSessions);

            this.recentFilesView = new ReadOnlyObservableCollectionView<string>(this.recentFiles);
        }

        /// <inheritdoc/>
        public event EventHandler<FileSessionEventArgs> Opened;

        /// <inheritdoc/>
        public event EventHandler<FileSessionEventArgs> Created;

        /// <inheritdoc/>
        public ReadOnlyObservableCollection<IFileSession> ActiveSessions
        {
            get
            {
                return this.activeSessionsView;
            }
        }

        /// <inheritdoc/>
        [Persistent]
        public ReadOnlyObservableCollectionView<string> RecentFiles
        {
            get
            {
                return this.recentFilesView;
            }
        }

        /// <summary>
        /// Lets the loader create an instance of this.
        /// </summary>
        public void Register()
        {            
        }

        /// <inheritdoc/>
        public IFileSession OnCreated(object origin, string extension)
        {
            if (origin == null)
            {
                throw new ArgumentNullException("origin");
            }

            if (extension == null)
            {
                throw new ArgumentNullException("extension");
            }

            if (extension.Substring(0, 2) != "*.")
            {
                throw new ArgumentException("The extension must be in format \"*.extension\".");
            }

            var session = new FileSession(this, extension);

            this.activeSessions.Add(session);

            if (this.Created != null)
            {
                var args = new FileSessionEventArgs(origin);
                this.Created(session, args);
            }

            return session;
        }

        /// <inheritdoc/>
        public IFileSession OnOpened(object origin, string fileName)
        {
            if (origin == null)
            {
                throw new ArgumentNullException("origin");
            }
            
            if (fileName == null)
            {
                throw new ArgumentNullException("fileName");
            }

            var session = new FileSession(this, fileName);

            this.activeSessions.Add(session);

            this.recentFiles.Add(fileName);

            if (this.Opened != null)
            {
                var args = new FileSessionEventArgs(origin);
                this.Opened(session, args);
            }

            return session;
        }
    }
}
