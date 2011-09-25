using System;
using System.Diagnostics.Contracts;
using System.Windows;
using System.Windows.Media;
using System.IO;
using System.Windows.Media.Imaging;
using System.ComponentModel;
using GeoGen.Studio.Utilities;

namespace GeoGen.Studio
{
    /// <summary>
    /// Advanced WPF wrapper around <see cref="GeoGen.Net.HeightData">GeoGen height map</see>. Supports application of <see cref="Overlay">overlays</see>.
    /// </summary>
    sealed public class HeightData: ObservableObject, INotifyPropertyChanged
    {
        private GeoGen.Net.HeightData heightData;

        #region Properties
        /// <summary>
        /// Gets height in given tile.
        /// </summary>
        /// <value></value>
        public int this[int x, int y]{
            get
            {
                return this.heightData[x, y];
            }
        }

        /// <summary>
        /// Name associated with the height map.
        /// </summary>
        /// <value>The name.</value>
        public string Name {get;set;}

        /// <summary>
        /// File from which was the height map loaded.
        /// </summary>
        /// <value>The name of the file.</value>
        public string FileName { get; private set; }        

        /// <summary>
        /// Width and height of the map.
        /// </summary>
        /// <value>The size.</value>
        public Size Size
        {
            get
            {
                return new Size(this.heightData.Width, this.heightData.Height);
            }
            set
            {                
                this.heightData = HeightData.GetResizedHeightData(this.heightData, (int) value.Width, (int) value.Height);
                this.RebuildImage(overlayOnly: false);
            }
        }

        private Overlay overlay;
        /// <summary>
        /// Gets or sets the <see cref="Overlay"/>.
        /// </summary>
        /// <value>The overlay.</value>
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

        /// <summary>
        /// Grayscale <see cref="ImageSource">image</see> of the height map.
        /// </summary>
        /// <value>The image.</value>
        public ImageSource Image {get; private set;}

        /// <summary>
        /// Colored <see cref="ImageSource">image</see> of the height map with applied <see cref="Overlay"/>.
        /// </summary>
        public ImageSource BaseImage { get; private set; }

        /// <summary>
        /// Gets minimum height in the map.
        /// </summary>
        /// <value>The min.</value>
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

        /// <summary>
        /// Maximum height in the map.
        /// </summary>
        /// <value>The max.</value>
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

        /// <summary>
        /// Initializes a new instance of the <see cref="HeightData"/> class from passed <see cref="GeoGen.Net.HeightData">GeoGen heigh tmap</see> object.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="heightData">The height data.</param>
        public HeightData(string name, GeoGen.Net.HeightData heightData) {
            this.Overlay = new Overlay();
            this.Name = name;
            this.heightData = heightData;
            this.ExpandValuesToFullRange();
            this.RebuildImage(overlayOnly: false);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="HeightData"/> class from passed height map file.
        /// </summary>
        /// <param name="path">The path.</param>
        /// <param name="width">The width.</param>
        /// <param name="height">The height.</param>
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
                // common image format .Net can read
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
                        this.heightData[i] = (short)((bytes[i * bytesPerPixel] - 128) * 255);
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

        /// <summary>
        /// Rebuilds the image(s) provided by this height map.
        /// </summary>
        /// <param name="overlayOnly">if set to <c>true</c>, only the overlaid image will be regenerated.</param>
        public void RebuildImage(bool overlayOnly = true){
            if (this.heightData == null) return;

            Byte[] bytes;

            // Don't redraw the base height map image if the height data didn't change.
            if(!overlayOnly){
                bytes = new Byte[this.heightData.Length];

                for (int i = 0; i < this.heightData.Length; i++){
                    bytes[i] = (byte)((this.heightData[i] / 256) + 128);
                }

                this.BaseImage = BitmapSource.Create(this.heightData.Width, this.heightData.Height, 96, 96, PixelFormats.Gray8, BitmapPalettes.Gray256, bytes, this.heightData.Width);
            }

            if (this.Overlay == null) return;
            
            bytes = new Byte[this.heightData.Length * 3];

            // Calculate the color value for each pixel.
            for (int i = 0; i < this.heightData.Length; i++)
            {
                short currentHeight = this.heightData[i];

                int overlayPixelIndex;
                if(this.Overlay.IsExtended)
                {
                    overlayPixelIndex = ((currentHeight / 128) + 255);

                    // Fix the water level being expanded into >0 levels by integer division artifacts.
                    if (currentHeight > 0 && overlayPixelIndex == 255)
                    {
                        overlayPixelIndex = 256;
                    }
                }
                else
                {
                    overlayPixelIndex = Math.Max(currentHeight / 128, 0);

                    // Fix the water level being expanded into >0 levels by integer division artifacts.
                    if(currentHeight > 0 && overlayPixelIndex == 0)
                    {
                        overlayPixelIndex = 1;
                    }
                }

                // Assign the color values by individual channels.
                bytes[i * 3] = this.Overlay.Bytes[overlayPixelIndex * 3];
                bytes[i * 3 + 1] = this.Overlay.Bytes[overlayPixelIndex * 3 + 1];
                bytes[i * 3 + 2] = this.Overlay.Bytes[overlayPixelIndex * 3 + 2];
            }

            this.Image = BitmapSource.Create(this.heightData.Width, this.heightData.Height, 96, 96, PixelFormats.Rgb24, BitmapPalettes.Halftone256, bytes, this.heightData.Width * 3);
        }

        /// <summary>
        /// Expands the values to full value range.
        /// </summary>
        public void ExpandValuesToFullRange()
        {
            int min = -this.Min;
            int max = this.Max;

            if (0 == min && 0 == max) return;

            double ratio = 1;
            if(min > max)
            {
                ratio = (double) short.MaxValue / (double) (min);                
            }
            else
            {
                ratio = (double) short.MaxValue / (double) max;
            }

            for(int i = 0; i < this.heightData.Length; i++)
            {
                this.heightData[i] = (short) (this.heightData[i] * ratio);
            }
        }
    }
}

