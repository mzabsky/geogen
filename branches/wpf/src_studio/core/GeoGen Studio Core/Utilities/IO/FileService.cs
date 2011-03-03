using System.IO;
using GeoGen.Studio.PlugIns;
using GeoGen.Studio.Utilities.Configurability;

namespace GeoGen.Studio.Utilities.IO
{    
    public static class FileService
    {
        [Configurable(UseEmptyInstanceAsDefault = true)]
        public static FileInfoObservableQueue RecentFiles { get; internal set; }

        public static event FileEventHandler FileOpened;
        public static event FileEventHandler FileClosed;
        public static event FileEventHandler FileSaved;

        static FileService()
        {
            FileService.RecentFiles = new FileInfoObservableQueue();
            MainConfig.RegisterStaticType(typeof (FileService));
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
