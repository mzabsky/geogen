namespace GeoGen.Studio.PlugIns.Services
{
    using System;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using System.Linq;

    using GeoGen.Studio.Utilities.Collections;
    using GeoGen.Studio.Utilities.Persistence;
    using GeoGen.Studio.Utilities.PlugInBase;

    // TODO: Persistence of RecentFiles.

    // TODO: Track owner of each session (only owner is allowed to change it and invoke operations on it).

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

            // TODO: This has to consider unit testing somehow.
            MainConfig.Register(this);
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
        /// Gets or sets the collection of persistent file. This is fake property for the persistence framework 
        /// (it requires property which is both readable and writable).
        /// </summary>
        [Persistent]
        protected IEnumerable<string> PersistentRecentFiles
        {
            get
            {
                // Items into the collection are added in reverse, compensate for this now.
                return this.RecentFiles.Reverse().ToList();
            }

            set
            {
                if (value != null)
                {
                    foreach (var item in value)
                    {
                        this.recentFiles.Add(item);
                    }
                }                
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
