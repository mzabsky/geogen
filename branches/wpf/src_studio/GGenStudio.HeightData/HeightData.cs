using System;
using System.Windows;
using System.Windows.Media;
using System.IO;
using System.Windows.Media.Imaging;

namespace GeoGen.Studio
{
    public class HeightData
    {
        protected GeoGen.Net.HeightData heightData;

        #region Properties
        public int this[int x, int y]{
            get
            {
                return this.heightData[x, y];
            }
        }

        public string Name {get;set;}

        public string FileName { get; protected set; }

        protected Size size;
        public Size Size
        {
            get
            {
                return this.size;
            }
            set
            {
                this.size = value;
                this.heightData = HeightData.GetResizedHeightData(this.heightData, (int)this.Size.Width, (int) this.Size.Height);
                this.RebuildImage(overlayOnly: false);
            }
        }

        protected string overlayFileName;
        public string OverlayFileName
        {
            get
            {
                return this.overlayFileName;
            }
            set
            {
                this.overlayFileName = value;
                this.RebuildImage(overlayOnly: true);
            }
        }

        public ImageSource Image {get; protected set;}

        public ImageSource BaseImage { get; protected set; }
        #endregion

        public HeightData() { }
        public HeightData(string path, double width, double height){
            if(!File.Exists(path)){
                throw new FileNotFoundException("Height data file \"" + path + "\" not found.");
            }            

            try{
                FileInfo fileInfo = new FileInfo(path);
                
                this.FileName = path;
                this.Name = fileInfo.Name;

                if (fileInfo.Extension == ".shd")
                {
                    // byte-by-byte binary reading
                    using(System.IO.BinaryReader reader = new System.IO.BinaryReader(System.IO.File.Open(path, System.IO.FileMode.Open, System.IO.FileAccess.Read))){

                        // read first eight bytes with map dimensions
                        int originalWidth = reader.ReadInt32();
                        int originalHeight = reader.ReadInt32();

                        this.heightData = new GeoGen.Net.HeightData(originalWidth, originalHeight);

                        // read the double bytes containing the height data
                        for (int i = 0; i < originalWidth * originalHeight; i++)
                        {
                            this.heightData[i] = reader.ReadInt16();
                        }

                        reader.Close();
                    }
                }
                else
                {
                    BitmapImage bitmap = new BitmapImage(new Uri(path));

                    this.heightData = new GeoGen.Net.HeightData(bitmap.PixelWidth, bitmap.PixelHeight);
                    int bytesPerPixel = ((bitmap.Format.BitsPerPixel + 7) / 8);
                    int stride = bitmap.PixelWidth * bytesPerPixel;
                    byte[] bytes = new byte[bitmap.PixelHeight * stride];
                    bitmap.CopyPixels(bytes, stride, 0);

                    for (int i = 0; i < bitmap.PixelWidth * bitmap.PixelHeight; i++)
                    {
                        this.heightData[i] = (short)(((int)bytes[i * bytesPerPixel] - 128)  * 255);
                    }
                }                
            }
            catch(Exception e){
                throw new FileNotFoundException("\"" + path + "\" is not a readable height map.", e);
            }

            this.Size = new Size(width, height); // property update callback will now resize the heightData and rebuild image
        }

        protected static GeoGen.Net.HeightData GetResizedHeightData(GeoGen.Net.HeightData data, int width, int height)
        {
            GeoGen.Net.HeightData resized = new GeoGen.Net.HeightData(width, height);

            // use nearest neighbor scaling algorithm
            for (int x = 0; x < width; x++)
            {
                for (int y = 0; y < height; y++)
                {
                    resized[x, y] = data[x * data.Width / width, y * data.Height / height];
                }
            }

            return resized;
        }

        public void RebuildImage(bool overlayOnly = true){
            Byte[] bytes;

            // don't redraw the base height map image if the height data didn't change
            if(!overlayOnly){
                bytes = new Byte[this.heightData.Length];

                for (int i = 0; i < this.heightData.Length; i++){
                    bytes[i] = (byte)((this.heightData[i] / 256) + 128);
                }

                this.BaseImage = BitmapSource.Create(this.heightData.Width, this.heightData.Height, 96, 96, PixelFormats.Gray8, BitmapPalettes.Gray256, bytes, (int)this.heightData.Width);
            }

            // load overlay file

            if (this.OverlayFileName == null) return;

            FileInfo fileInfo = new FileInfo(this.OverlayFileName);

            BitmapImage overlayBitmap = new BitmapImage(new Uri(fileInfo.FullName));

            FormatConvertedBitmap convertedOverlayBitmap = new FormatConvertedBitmap(overlayBitmap, PixelFormats.Rgb24, BitmapPalettes.Halftone256, 0);

            int overlayStride = convertedOverlayBitmap.PixelWidth * 3;
            byte[] overlayBytes = new byte[convertedOverlayBitmap.PixelHeight * overlayStride];
            convertedOverlayBitmap.CopyPixels(overlayBytes, overlayStride, 0);

            bool extendedOverlay;

            if (convertedOverlayBitmap.PixelWidth == 256)
            {
                extendedOverlay = false;
            }
            else if (convertedOverlayBitmap.PixelWidth == 511)
            {
                extendedOverlay = true;
            }
            else{
                throw new InvalidOperationException("Overlay \"" + this.OverlayFileName + "\"has incorrect width.");
            }
            
            bytes = new Byte[this.heightData.Length * 3];

            for (int i = 0; i < this.heightData.Length; i++)
            {
                // red
                bytes[i * 3] = overlayBytes[(extendedOverlay ? (
                        (this.heightData[i] / 128) + 256
                    ) : (
                        Math.Max(this.heightData[i] / 128, 0)
                    )) * 3];

                // green
                bytes[i * 3 + 1] = overlayBytes[(extendedOverlay ? (
                        (this.heightData[i] / 128) + 256
                    ) : (
                        Math.Max(this.heightData[i] / 128, 0)
                    )) * 3 + 1];

                //blue
                bytes[i * 3 + 2] = overlayBytes[(extendedOverlay ? (
                        (this.heightData[i] / 128) + 256
                    ) : (
                        Math.Max(this.heightData[i] / 128, 0)
                    )) * 3 + 2];
            }

            this.Image = BitmapSource.Create(this.heightData.Width, this.heightData.Height, 96, 96, PixelFormats.Rgb24, BitmapPalettes.Halftone256, bytes, this.heightData.Width * 3);
        }
    }
}

