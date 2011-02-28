using System.IO;
using GeoGen.Studio.Utilities.Collections;

namespace GeoGen.Studio.PlugIns
{
    public static class FileService
    {
        public static UniqueObservableQueue<FileInfo> RecentFiles { get; private set; }

        public static event FileEventHandler FileOpened;
        public static event FileEventHandler FileClosed;
        public static event FileEventHandler FileSaved;

        static FileService()
        {
            FileService.RecentFiles = new UniqueObservableQueue<FileInfo>();
        }

        public static void OnFileOpened(object sender, FileInfo fileInfo)
        {
            FileService.RecentFiles.Add(fileInfo);

            if(FileService.FileOpened != null)
            {
                FileService.FileOpened(sender, new FileEventArgs(fileInfo));
            }
        }

        public static void OnFileSaved(object sender, FileInfo fileInfo)
        {
            FileService.RecentFiles.Add(fileInfo);

            if (FileService.FileSaved != null)
            {
                FileService.FileSaved(sender, new FileEventArgs(fileInfo));
            }
        }

        public static void OnFileClosed(object sender, FileInfo fileInfo)
        {
            if (FileService.FileClosed != null)
            {
                FileService.FileClosed(sender, new FileEventArgs(fileInfo));
            }
        }
    }
}
