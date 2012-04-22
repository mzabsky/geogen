namespace GeoGen.Studio.PlugIns.Services
{
    using System;
    using System.Collections.ObjectModel;

    using GeoGen.Studio.PlugInLoader;
    using GeoGen.Studio.Utilities.Collections;

    /// <summary>
    /// Manages files opened by various plug-ins.
    /// </summary>
    public interface IFileService : IPlugInInterface
    {
        /// <summary>
        /// Occurs when <see cref="FileService.OnOpened"/> is called.
        /// </summary>
        event EventHandler<FileSessionEventArgs> Opened;

        /// <summary>
        /// Occurs when <see cref="OnCreated"/> is called.
        /// </summary>
        event EventHandler<FileSessionEventArgs> Created;

        /// <summary>
        /// Gets the collection of all currently active (opened/created and not closed) sessions owned by this service.
        /// </summary>
        ReadOnlyObservableCollection<IFileSession> ActiveSessions { get; }

        /// <summary>
        /// Gets the collection of recently saved/opened files owned by this service.
        /// </summary>
        ReadOnlyObservableCollectionView<string> RecentFiles { get; }

        /// <summary>
        /// Creates new file session which is not bound to any file-system file.
        /// </summary>
        /// <param name="origin">The origin.</param>
        /// <param name="extension">The extension in format ".extension".</param>
        /// <returns>The session.</returns>
        IFileSession OnCreated(object origin, string extension);

        /// <summary>
        /// To be caled when a file is first opened. A new session is created for this file.
        /// </summary>
        /// <param name="origin">The origin.</param>
        /// <param name="fileName">Name of the file.</param>
        /// <returns>The session.</returns>
        IFileSession OnOpened(object origin, string fileName);
    }
}