using System;
using System.ComponentModel;
using System.Diagnostics.Contracts;
using System.IO;

namespace GeoGen.Studio.Utilities.IO
{
    /// <summary>
    /// Information about one opened file.
    /// </summary>
    public class FileSession: ObservableObject, INotifyPropertyChanged
    {
        private FileInfo fileInfo;
        /// <summary>
        /// Info of the file.
        /// </summary>
        public FileInfo FileInfo 
        {
            get
            {
                return this.fileInfo;
            }

            internal set
            {
                Contract.Requires(value != null);
                this.fileInfo = value;
                this.Extension = value.Extension;
            }
        }

        /// <summary>
        /// The file session <see cref="Guid"/>.
        /// </summary>
        public Guid FileSessionGuid { get; internal set; }


        /// <summary>
        /// Extension of the file.
        /// </summary>
        public string Extension { get; private set; }

        internal FileSession(FileInfo fileInfo)
        {
            Contract.Requires(fileInfo != null);

            this.fileInfo = fileInfo;
            this.FileSessionGuid = Guid.NewGuid();
        }

        internal FileSession(string extension)
        {
            this.Extension = extension;
            this.FileSessionGuid = Guid.NewGuid();
        }
    }
}
