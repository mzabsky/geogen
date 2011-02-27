using System;
using System.Windows;
using System.Windows.Media;
using System.IO;
using System.Windows.Media.Imaging;
using System.ComponentModel;

namespace GeoGen.Studio
{
    sealed public class HeightData: INotifyPropertyChanged
    {
        private GeoGen.Net.HeightData heightData;

        public event PropertyChangedEventHandler PropertyChanged;

        #region Properties
        public int this[int x, int y]{
            get
            {
                return this.heightData[x, y];
            }
        }

        public string Name {get;set;}

        public string FileName { get; private set; }

        public Size Size
        {
            get
            {
                return new Size(this.heightData.Width, this.heightData.Height);
            }
            set
            {                
                this.heightData = HeightData.GetResizedHeightData(this.heightData, (int)this.Size.Width, (int) this.Size.Height);
                this.RebuildImage(overlayOnly: false);
            }
        }

        private Overlay overlay;
        public Overlay Overlay
        {
            get
            {
                return this.overlay;
            }
            set
            {
                this.overlay = value;
                this.RebuildImage(overlayOnly: true);
            }
        }

        public ImageSource Image {get; private set;}

        public ImageSource BaseImage { get; private set; }

        public int Min
        {
            get
            {
                int min = Int16.MaxValue;
                for (int i = 0; i < heightData.Length; i++)
                {
                    if (heightData[i] < min) min = heightData[i];
                }
                return min;
            }
        }

        public int Max
        {
            get
            {
                int max = Int16.MinValue;
                for (int i = 0; i < heightData.Length; i++)
                {
                    if (heightData[i] > max) max = heightData[i];
                }
                return max;
            }
        }
        #endregion

        public HeightData(string name, GeoGen.Net.HeightData heightData) {
            this.Name = name;
            this.heightData = heightData;
            this.ExpandValuesToFullRange();
            this.RebuildImage(overlayOnly: false);
        }

        public HeightData(string path, double width, double height)
        {
            if (!File.Exists(path))
            {
                throw new FileNotFoundException("Height data file \"" + path + "\" not found.");
            }

            try
            {
                FileInfo fileInfo = new FileInfo(path);

                this.FileName = path;
                this.Name = fileInfo.Name;

                if (fileInfo.Extension == ".shd")
                {
                    // byte-by-byte binary reading
                    using (BinaryReader reader = new BinaryReader(File.Open(path, FileMode.Open, FileAccess.Read)))
                    {

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
                        this.heightData[i] = (short)(((int)bytes[i * bytesPerPixel] - 128) * 255);
                    }
                }
            }
            catch (Exception e)
            {
                throw new FileNotFoundException("\"" + path + "\" is not a readable height map.", e);
            }

            this.Size = new Size(width, height); // property update callback will now resize the heightData and rebuild image
        }

        private static GeoGen.Net.HeightData GetResizedHeightData(GeoGen.Net.HeightData data, int width, int height)
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

            if (this.Overlay == null) return;
            
            bytes = new Byte[this.heightData.Length * 3];

            for (int i = 0; i < this.heightData.Length; i++)
            {
                // red
                bytes[i * 3] = this.Overlay.Bytes[(this.Overlay.IsExtended ? (
                        (this.heightData[i] / 128) + 256
                    ) : (
                        Math.Max(this.heightData[i] / 128, 0)
                    )) * 3];

                // green
                bytes[i * 3 + 1] = this.Overlay.Bytes[(this.Overlay.IsExtended ? (
                        (this.heightData[i] / 128) + 256
                    ) : (
                        Math.Max(this.heightData[i] / 128, 0)
                    )) * 3 + 1];

                //blue
                bytes[i * 3 + 2] = this.Overlay.Bytes[(this.Overlay.IsExtended ? (
                        (this.heightData[i] / 128) + 256
                    ) : (
                        Math.Max(this.heightData[i] / 128, 0)
                    )) * 3 + 2];
            }

            this.Image = BitmapSource.Create(this.heightData.Width, this.heightData.Height, 96, 96, PixelFormats.Rgb24, BitmapPalettes.Halftone256, bytes, this.heightData.Width * 3);
        }

        public void ExpandValuesToFullRange()
        {
            int min = this.Min;
            int max = this.Max;

            if (0 == min && 0 == max) return;

            double ratio = 1;
            if(min > max)
            {
                ratio = (double) short.MaxValue / (double) min;
            }
            else
            {
                ratio = (double) short.MaxValue / (double)max;
            }

            for(int i = 0; i < this.heightData.Length; i++)
            {
                this.heightData[i] = (short) (this.heightData[i] * ratio);
            }
        }

        private void OnPropertyChanged(string info)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(info));
            }
        }
    }
}

