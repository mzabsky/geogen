using System.Windows.Media;
using System.IO;
using System.Windows.Media.Imaging;
using GeoGen.Studio.Utilities.Messaging;
using System.Collections.Generic;
using System;

namespace GeoGen.Studio.Utilities.IO
{
	public class BitmapWriter
	{
		public bool IsFileDialogEnabled {get; set;}

		public bool Is16Bit {get; set;}

		public bool IsGrayscale {get; set;}

		public string SaveBitmap(BitmapSource bitmap, string path)
		{
			var convertedBitmap = this.ConvertBitmap(bitmap);

			if (this.IsFileDialogEnabled)
			{
				path = FileDialog.ShowSave(path, this.GetDialogFilterString());
			}

			// Create image encoder for given image format.
			BitmapEncoder encoder = this.GetBitmapEncoder(Path.GetExtension(path));
			BitmapFrame frame = BitmapFrame.Create(convertedBitmap);
			encoder.Frames.Add(frame);

			encoder.Save(new FileStream(path, FileMode.OpenOrCreate, FileAccess.Write));

			return path;
		}

		public BitmapEncoder GetBitmapEncoder(string extension)
		{
			switch (extension.ToLower())
			{
				case ".jpg":
				case ".jpeg":
					return this.GetJpegEncoder();
				case ".png":
					return this.GetPngEncoder();
				case ".bmp":
					return this.GetBmpEncoder();
				case ".tiff":
				case ".tif":
					return this.GetTiffEncoder();
				case ".gif":
					return this.GetGifEncoder();
				default:
					throw new IOException(@"Image encoder did not recognize file type """ + extension + @""".");
			}
		}

		private JpegBitmapEncoder GetJpegEncoder()
		{
			JpegBitmapEncoder encoder = new JpegBitmapEncoder();
			encoder.QualityLevel = 100;

			return encoder;
		}

		private PngBitmapEncoder GetPngEncoder()
		{
			PngBitmapEncoder encoder = new PngBitmapEncoder();
			return encoder;
		}

		private BmpBitmapEncoder GetBmpEncoder()
		{
			BmpBitmapEncoder encoder = new BmpBitmapEncoder();
			return encoder;
		}

		private TiffBitmapEncoder GetTiffEncoder()
		{
			TiffBitmapEncoder encoder = new TiffBitmapEncoder();
			return encoder;
		}

		private GifBitmapEncoder GetGifEncoder()
		{
			if (this.Is16Bit)
			{
				throw new IOException(@"GIF file format doesn't support 16 bits per channel.");
			}

			GifBitmapEncoder encoder = new GifBitmapEncoder();
			return encoder;
		}

		private BitmapSource ConvertBitmap(BitmapSource bitmap)
		{
			FormatConvertedBitmap convertedBitmap = new FormatConvertedBitmap();
			convertedBitmap.BeginInit();
			convertedBitmap.Source = bitmap;
			convertedBitmap.DestinationFormat = this.GetPixelFormat();
			convertedBitmap.EndInit();

			return convertedBitmap;
		}

		private PixelFormat GetPixelFormat()
		{
			if (this.Is16Bit && this.IsGrayscale)
			{
				return PixelFormats.Gray16;
			}
			else if (this.Is16Bit && !this.IsGrayscale)
			{
				return PixelFormats.Rgb48;
			}
			else if (this.IsGrayscale)
			{
				return PixelFormats.Gray8;
			}
			else
			{
				return PixelFormats.Rgb24;
			}
		}

		private string GetDialogFilterString(){
			List<string> filterStrings = new List<string>();

			filterStrings.Add("BMP - Windows Bitmap (*.bmp)|*.bmp");

			if (!this.Is16Bit) {
				filterStrings.Add("GIF - Graphics Interchangeable Format (*.gif)|*.gif");
			}

			if (!this.IsGrayscale) {
				// Grayscale mode usually means a height map is being exported. We don't want artifacts there.
				filterStrings.Add("JPG - JPEG Image (*.jpg)|*.jpg");
			}
			filterStrings.Add("PNG - Portable Network Graphics (*.png)|*.png");
			filterStrings.Add("JPG - JPEG Image (*.jpg)|*.jpg");
			filterStrings.Add("TIF - Tagged Image File (*.tif)|*.tif");
			filterStrings.Add("All files (*.*)|*.*");

			return String.Join("|", filterStrings);
		}
	}
}
