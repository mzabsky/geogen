using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.IO;
using System.Linq;
using GeoGen.Studio.Utilities.Collections;
using GeoGen.Studio.Utilities.Persistence;

namespace GeoGen.Studio.Utilities.IO
{    
    public static class FileService
    {
        /// <summary>
        /// List of recently used files.
        /// </summary>
        [Persistent(UseEmptyInstanceAsDefault = true)]
        public static FileInfoObservableQueue RecentFiles { get; internal set; }

        /// <summary>
        /// List of currently opened files.
        /// </summary>
        public static ICollection<FileSession> OpenedFiles { get; internal set; }

        public static event FileEventHandler FileCreated;
        public static event FileEventHandler FileOpened;
        public static event FileEventHandler FileClosed;
        public static event FileEventHandler FileSaved;        

        static FileService()
        {
            FileService.RecentFiles = new FileInfoObservableQueue();
            FileService.OpenedFiles = new List<FileSession>();
            MainConfig.RegisterStaticType(typeof (FileService));
        }

        public static FileSession GetFileSessionByGuid(Guid guid)
        {
            return FileService.OpenedFiles.Where(fileSession => fileSession.FileSessionGuid == guid).Single();
        }

        public static Guid OnFileCreated(object sender, string extension)
        {
            Contract.Requires(extension != null);
            
            FileSession fileSession = new FileSession(extension);

            FileService.OpenedFiles.Add(fileSession);

            if(FileService.FileCreated != null)
            {
                FileService.FileCreated(sender, new FileEventArgs(fileSession));
            }

            return fileSession.FileSessionGuid;
        }

        public static Guid OnFileOpened(object sender, FileInfo fileInfo)
        {
            Contract.Requires(fileInfo != null);            

            FileSession fileSession = new FileSession(fileInfo);
            FileService.OpenedFiles.Add(fileSession);

            FileService.RecentFiles.Add(fileInfo);
            
            if(FileService.FileOpened != null)
            {
                FileService.FileOpened(sender, new FileEventArgs(fileSession));
            }

            return fileSession.FileSessionGuid;
        }

        public static void OnFileSaved(object sender, Guid fileSessionGuid, FileInfo fileInfo)
        {
            Contract.Requires(fileInfo != null);

            FileSession fileSession = FileService.GetFileSessionByGuid(fileSessionGuid);            

            fileSession.FileInfo = fileInfo;
            
            FileService.RecentFiles.Add(fileInfo);

            if (FileService.FileSaved != null)
            {
                FileService.FileSaved(sender, new FileEventArgs(fileSession));
            }
        }

        public static void OnFileClosed(object sender, Guid fileSessionGuid)
        {
            FileSession fileSession = FileService.GetFileSessionByGuid(fileSessionGuid);
            FileService.OpenedFiles.Remove(fileSession);

            if (FileService.FileClosed != null)
            {
                FileService.FileClosed(sender, new FileEventArgs(fileSession));
            }
        }
    }
}
