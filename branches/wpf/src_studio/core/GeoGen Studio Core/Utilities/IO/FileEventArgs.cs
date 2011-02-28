using System;
using System.IO;

namespace GeoGen.Studio.PlugIns
{
    public delegate void FileEventHandler (object sender, FileEventArgs args);

    public class FileEventArgs: EventArgs
    {
        public FileInfo FileInfo { get; private set; }

        public FileEventArgs(FileInfo file)
        {
            this.FileInfo = file;
        }
    }
}
