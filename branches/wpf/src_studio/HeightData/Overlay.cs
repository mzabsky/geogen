using System;
using System.IO;
using System.Collections.Generic;
using System.Windows.Media.Imaging;
using System.Xml;
using System.Xml.Serialization;
using System.Xml.Schema;
using System.Windows.Media;
using GeoGen.Studio.Utilities.Messaging;

namespace GeoGen.Studio
{
    using System.Linq;

    sealed public class Overlay: IXmlSerializable
    {
        public static string OverlayDirectory
        {
            get
            {
                return "../overlays/";
            }
        }

        public static string DefaultOverlayName
        {
            get
            {
                return "TopoBathy.bmp";
            }
        }

        private static IEnumerable<Overlay> all;

        /// <summary>
        /// Returns all valid overlays found in the overlay directory.
        /// </summary>
        /// <value>All.</value>
        public static IEnumerable<Overlay> All
        {
            get
            {
                if(Overlay.all != null)
                {
                    // The overlays have already been loaded from the hard drive
                    return Overlay.all;
                }

                List<Overlay> overlays = new List<Overlay>();

                DirectoryInfo directoryInfo = new DirectoryInfo(Overlay.OverlayDirectory);

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
                        new Message("Could not open overlay \"" + overlayFile.Name + "\".", MessageType.Error).Send();
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

        public FileInfo FileInfo { get; private set; }

        public BitmapSource Bitmap {get; private set;}
        
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
                return this.Bitmap.PixelWidth == 511;
            }
        }

        public Overlay()
        {
            this.LoadFromFile(new FileInfo(Overlay.OverlayDirectory + Overlay.DefaultOverlayName));
        }

        public Overlay(FileInfo file)
        {            
            this.LoadFromFile(file);
        }

        private void LoadFromFile(FileInfo file)
        {
            FormatConvertedBitmap convertedBitmap = new FormatConvertedBitmap(new BitmapImage(new Uri(file.FullName)), PixelFormats.Rgb24, BitmapPalettes.Halftone256, 1);

            this.Bitmap = convertedBitmap;             
            this.FileName = file.FullName;
            this.Name = file.Name;
            this.FileInfo = file;

            if(this.Bitmap.PixelHeight != 1 || (this.Bitmap.PixelWidth != 511 && this.Bitmap.PixelWidth != 256))
            {
                throw new ArgumentException("Incorrect overlay size.");
            }

            // copy the color data into the Bytes array
            int stride = this.Bitmap.PixelWidth * this.BytesPerPixel;
            this.Bytes = new byte[this.Bitmap.PixelHeight * stride];
            this.Bitmap.CopyPixels(this.Bytes, stride, 0);
        }

        public override string ToString()
        {
            return this.Name;
        }

        public override bool Equals(object obj)
        {
            var overlay = obj as Overlay;

            if (overlay == null)
            {
                return false;
            }

            return overlay.Name == this.Name;
        }

        public override int GetHashCode()
        {
            return this.Name.GetHashCode();
        }

        public XmlSchema GetSchema()
        {
            return null;
        }

        public void ReadXml(XmlReader reader)
        {
            reader.ReadStartElement();
            this.LoadFromFile(new FileInfo(Overlay.OverlayDirectory + reader.ReadString()));
            reader.ReadEndElement();
        }

        public void WriteXml(XmlWriter writer)
        {
            writer.WriteString(this.Name);
        }
    }
}
