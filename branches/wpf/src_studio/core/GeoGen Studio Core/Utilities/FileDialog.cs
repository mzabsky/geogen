using System;
using Microsoft.Win32;
using System.IO;

namespace GeoGen.Studio.Utilities
{
    public static class FileDialog
    {
        private static string ShowBase(string path, string filter, bool isSaveDialog)
        {
            Microsoft.Win32.FileDialog dialog;

            if (isSaveDialog) dialog = new SaveFileDialog();
            else dialog = new OpenFileDialog();

            dialog.Filter = filter;

            // add Examples shortcut, if the directory exists
            if (Directory.Exists(@"../examples"))
            {
                dialog.CustomPlaces.Add(new FileDialogCustomPlace(@"../examples"));
            }

            try
            {
                FileInfo info = new FileInfo(path);
                dialog.InitialDirectory = info.DirectoryName;
                dialog.FileName = info.Name;
            }
            // path  is incorrect -> let the dialog reset on defaults
            catch { };

            bool? result = dialog.ShowDialog();

            if (result == true)
            {
                path = dialog.FileName;

                return path;
            }

            // user pressed cancel
            throw new Exception();
        }

        public static string ShowOpen(string path, string filter)
        {
            return FileDialog.ShowBase(path, filter, false);
        }

        public static string ShowSave(string path, string filter)
        {
            return FileDialog.ShowBase(path, filter, true);
        }
    }
}
