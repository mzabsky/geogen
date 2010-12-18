using System;
using System.IO;
using System.Collections.Generic;
using System.Windows.Media.Imaging;

namespace GeoGen.Studio
{
    sealed public class Overlay
    {
        private static IEnumerable<Overlay> all;

        /// <summary>
        /// Returns all valid overlays found in the overlay directory.
        /// </summary>
        /// <value>All.</value>
        private static IEnumerable<Overlay> All
        {
            get
            {
                if(Overlay.all != null)
                {
                    // The overlays have already been loaded from the hard drive
                    return Overlay.all;
                }

                List<Overlay> overlays = new List<Overlay>();

                DirectoryInfo directoryInfo = new DirectoryInfo("../overlays");

                // Load all overlay files in the directory
                foreach(FileInfo overlayFile in directoryInfo.GetFiles("*.bmp"))
                {
                    try
                    {
                        overlays.Add(new Overlay(overlayFile));
                    }
                    catch(Exception)
                    {
                        // The overlay might have been invalid
                        continue;
                    }
                }

                // Do not allow any changes to the list
                Overlay.all = overlays.AsReadOnly();

                return Overlay.all;
            }
        }

        public string Name {get; private set;}

        public string FileName { get; private set; }

        public BitmapImage Bitmap {get; private set;}
        
        public int BytesPerPixel {
            get
            {
                return ((this.Bitmap.Format.BitsPerPixel + 7) / 8);
            }
        }

        public byte[] Bytes {get; private set;}

        internal Overlay(FileInfo file)
        {
            this.Bitmap = new BitmapImage(new Uri(file.FullName));
            this.FileName = file.FullName;
            this.Name = file.Name;
        }
    }
}
