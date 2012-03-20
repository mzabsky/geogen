namespace GeoGen.Studio.Utilities.Extensions
{
    using System;
    using System.IO;
    using System.Runtime.InteropServices;

    using GeoGen.Studio.Utilities.Messaging;

    /// <summary>
    /// Extension methods for <see cref="FileInfo"/> class.
    /// </summary>
    public static class FileInfoExtensions
    {
        /// <summary>
        /// Gets mime type of a file.
        /// </summary>
        /// <param name="fileInfo">The file info.</param>
        /// <returns>The mime type.</returns>
        public static string GetMimeType(this FileInfo fileInfo)
        {
            var buffer = new byte[256];

            // Grab the first 256 bytes on the file.
            using (var fileStream = new FileStream(fileInfo.FullName, FileMode.Open))
            {
                if (fileStream.Length >= 256)
                {
                    fileStream.Read(buffer, 0, 256);
                }
                else
                {
                    fileStream.Read(buffer, 0, (int)fileStream.Length);
                }
            }

            try
            {
                uint mimeType;
                FindMimeFromData(0, null, buffer, 256, null, 0, out mimeType, 0);
                var mimeTypePointer = new IntPtr(mimeType);
                return Marshal.PtrToStringUni(mimeTypePointer);
            }
            catch (Exception)
            {               
                new Message(
                    string.Format("Could not identify file \"{0}\".", fileInfo.Name), 
                    MessageType.Error).Send();

                return string.Empty;
            }
        }

        /// <summary>
        /// Determines whether the file is a text file.
        /// </summary>
        /// <param name="fileInfo">The file info.</param>
        /// <returns>
        ///   <c>true</c> if the file is a text file; otherwise, <c>false</c>.
        /// </returns>
        public static bool IsTextFile(this FileInfo fileInfo)
        {
            return fileInfo.GetMimeType().IndexOf("text/", System.StringComparison.Ordinal) == 0;
        }

        /// <summary>
        /// Determines whether the specified file is image.
        /// </summary>
        /// <param name="fileInfo">The file info.</param>
        /// <returns>
        ///   <c>true</c> if the specified file info is image; otherwise, <c>false</c>.
        /// </returns>
        public static bool IsImage(this FileInfo fileInfo)
        {
            return fileInfo.GetMimeType().IndexOf("image/", System.StringComparison.Ordinal) == 0;
        }

        [DllImport(@"urlmon.dll", CharSet = CharSet.Auto)]
        private extern static uint FindMimeFromData(
             uint pBC,
             [MarshalAs(UnmanagedType.LPStr)] string pwzUrl,
             [MarshalAs(UnmanagedType.LPArray)] byte[] pBuffer,
             uint cbSize,
             [MarshalAs(UnmanagedType.LPStr)] string pwzMimeProposed,
             uint dwMimeFlags,
             out uint ppwzMimeOut,
             uint dwReserved);
    }
}
