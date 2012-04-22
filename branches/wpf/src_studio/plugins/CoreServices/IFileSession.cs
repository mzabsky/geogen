namespace GeoGen.Studio.PlugIns.Services
{
    using System;

    /// <summary>
    /// Represents one file managed by <see cref="IFileService"/>.
    /// </summary>
    public interface IFileSession
    {
        /// <summary>
        /// Occurs when <see cref="IFileSession.OnChanged"/> is called.
        /// </summary>
        event EventHandler<FileSessionEventArgs> Changed;

        /// <summary>
        /// Occurs when <see cref="IFileSession.OnClosed"/> or <see cref="IFileSession.OnSavedAs"/> is called.
        /// </summary>
        event EventHandler<FileSessionEventArgs> Closed;

        /// <summary>
        /// Occurs when <see cref="IFileSession.OnSaved"/> is called.
        /// </summary>
        event EventHandler<FileSessionEventArgs> Saved;

        /// <summary>
        /// Gets the name of the file to which this session is bound. This property is immutable> Functions that would change this (<see cref="OnSavedAs"/>)
        /// return a new session instead of changing this and close the original session.
        /// </summary>
        /// <value>
        /// The name of the file.
        /// </value>
        string FileName { get; }

        /// <summary>
        /// Gets the extension of <see cref="FileName"/> in format ".extension".
        /// </summary>
        string Extension { get; }

        /// <summary>
        /// Gets a value indicating whether this instance is unsaved.
        /// </summary>
        /// <value>
        /// <c>true</c> if this instance is unsaved; otherwise, <c>false</c>.
        /// </value>
        bool IsUnsaved { get; }

        /// <summary>
        /// Gets a value indicating whether this session was already closed. No operations are allowed once the session is closed.
        /// </summary>
        bool IsClosed { get; }

        /// <summary>
        /// Gets a value indicating whether this session corresponds to actual file on the file system. Session that have been just created, 
        /// but not saved yet have this set to <c>false</c>.
        /// </summary>
        /// <value>
        ///   <c>true</c> if this instance is bound; otherwise, <c>false</c>.
        /// </value>
        bool IsBound { get; }

        /// <summary>
        /// To be called when the file represented by the session is changed.
        /// </summary>
        /// <param name="origin">The object causing the change.</param>
        void OnChanged(object origin);

        /// <summary>
        /// To be called when the file represented by the session is closed.
        /// </summary>
        /// <param name="origin">The object causing the change.</param>
        void OnClosed(object origin);

        /// <summary>
        /// To be called when the file represented by the session is saved.
        /// </summary>
        /// <param name="origin">The object causing the change.</param>
        void OnSaved(object origin);

        /// <summary>
        /// To be called when the file represented by the session is saved as another file.
        /// </summary>
        /// <param name="origin">The object causing the change.</param>
        /// <param name="fileName">File name as which the file is saved</param>
        /// <returns><see cref="IFileSession"/> representing the another file.</returns>
        IFileSession OnSavedAs(object origin, string fileName);
    }
}