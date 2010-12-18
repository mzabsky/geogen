using System;
using System.IO;
using System.Collections.Generic;
using System.Windows.Media.Imaging;
using GeoGen.Studio.Utilities.Messaging;

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
                        Messenger.ThrowMessage(new Message("Could not open overlay \"" + overlayFile.Name + "\".", MessageType.Error));
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

        public bool IsExtended
        {
            get
            {
                return this.Bitmap.Width == 511;
            }
        }

        internal Overlay(FileInfo file)
        {            
            this.Bitmap = new BitmapImage(new Uri(file.FullName));
            this.FileName = file.FullName;
            this.Name = file.Name;

            if(this.Bitmap.Height != 1 || (this.Bitmap.Width != 511 && this.Bitmap.Width != 256))
            {
                throw new ArgumentException("Incorrect overlay size.");
            }

            // copy the color data into the Bytes array
            int stride = this.Bitmap.PixelWidth * this.BytesPerPixel;
            this.Bytes = new byte[this.Bitmap.PixelHeight * stride];
            this.Bitmap.CopyPixels(this.Bytes, stride, 0);
        }
    }
}
