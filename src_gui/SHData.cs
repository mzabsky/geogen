/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    GeoGen is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GeoGen.  If not, see <http://www.gnu.org/licenses/>.

*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GeoGen_Studio
{
    public class SHData
    {
        public Int16[] data;
        public int width;
        public int height;
        public int ByteLength
        {
            get { return sizeof(Int16) * width * height; }
        }

        public SHData() { }

        public SHData(string path)
        {
            string ext = path.Substring(path.LastIndexOf('.'), path.Length - path.LastIndexOf('.')).ToLower();

            if (ext == ".shd")
            {
                // byte-by-byte binary reading
                System.IO.BinaryReader reader = new System.IO.BinaryReader(System.IO.File.Open(path, System.IO.FileMode.Open, System.IO.FileAccess.Read));

                // read first eight bytes with map dimensions
                this.width = reader.ReadInt32();
                this.height = reader.ReadInt32();

                // prepare memory space for the map data
                this.data = new Int16[this.width * this.height];

                // read the double bytes containing the height data
                for (int i = 0; i < this.width * this.height; i++)
                {
                    this.data[i] = reader.ReadInt16();
                }

                reader.Close();
            }
            else
            {
                // read the bitmap file
                System.Drawing.Bitmap bitmap = new System.Drawing.Bitmap(path);
                System.Drawing.Rectangle rect = new System.Drawing.Rectangle(0, 0, bitmap.Width, bitmap.Height);
                System.Drawing.Imaging.BitmapData data = bitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, bitmap.PixelFormat);

                this.width = bitmap.Width;
                this.height = bitmap.Height;
                this.data = new Int16[this.width * this.height];

                // prepare memory space for the color data
                byte[] bytes = new byte[data.Stride * bitmap.Height];

                // get a pointer to the to first line (=first pixel)
                IntPtr ptr = data.Scan0;

                // create a byte copy of the heightmap data
                System.Runtime.InteropServices.Marshal.Copy(ptr, bytes, 0, data.Stride * bitmap.Height);

                // create the color data

                for (int i = 0; i < bytes.Length; i += 4)
                {
                    this.data[i / 4] = (short)((short)bytes[i] * 128);
                }

                bitmap.UnlockBits(data);
            }
        }

        public System.Drawing.Bitmap GetBitmap()
        {
            // create a blank bitmap
            System.Drawing.Bitmap bitmap = new System.Drawing.Bitmap(this.width, this.height);

            // lock the bitmap color data for byte access
            System.Drawing.Rectangle rect = new System.Drawing.Rectangle(0, 0, bitmap.Width, bitmap.Height);
            System.Drawing.Imaging.BitmapData locked = bitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.WriteOnly, bitmap.PixelFormat);

            // prepare memory space for the new color data
            byte[] bytes = new byte[locked.Stride * bitmap.Height];

            // get a pointer to the to first line (=first pixel)
            IntPtr ptr = locked.Scan0;

            // fill in the bytes
            for (int i = 0; i < bytes.Length; i += 4)
            {
                // we won't be able to display the height data lower than 0 -> crop them
                byte current = (byte)(this.data[i / 4] > 0 ? (this.data[i / 4] / 128) : 0);

                bytes[i + 0] = current;
                bytes[i + 1] = current;
                bytes[i + 2] = current;
                bytes[i + 3] = 255;
            }

            // copy the data into the bitmap
            System.Runtime.InteropServices.Marshal.Copy(bytes, 0, ptr, locked.Stride * bitmap.Height);

            // unlock the bits
            bitmap.UnlockBits(locked);

            return bitmap;
        }

        public SHData GetResized(int width, int height)
        {
            SHData resized = new SHData();
            resized.width = width;
            resized.height = height;
            resized.data = new Int16[width * height];

            // use nearest neighbor scaling algorithm
            for (int x = 0; x < width; x++)
            {
                for (int y = 0; y < height; y++)
                {
                    resized.data[x + width * y] = data[x * this.width / width + y * this.height / height * this.width];
                }
            }

            return resized;
        }

        public void Export()
        {
            Main main = Main.Get();
            Config config = main.GetConfig();

            Export export =  new Export();
            export.width.Maximum = this.width;
            export.width.Value = this.width;
            export.height.Maximum = this.height;
            export.height.Value = this.height;
            if (config.exportRescaleMode) export.subzeroMode2.Checked = true;

            if (export.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                if (main.FileDialog(main.exportHeightmapDialog, ref config.lastExportedFile))
                {
                    string path = main.exportHeightmapDialog.FileName;
                    string ext = path.Substring(path.LastIndexOf('.'), path.Length - path.LastIndexOf('.')).ToLower();

                    config.lastExportedFile = path;

                    if (ext == ".shd")
                    {
                        SHData toExport = null;

                        if (this.width != export.width.Value || this.height != export.height.Value)
                        {
                            toExport = this.GetResized((int)export.width.Value, (int) export.height.Value);
                        }
                        else
                        {
                            toExport = this;
                        }

                        System.IO.BinaryWriter writer = new System.IO.BinaryWriter(System.IO.File.Open(path, System.IO.FileMode.OpenOrCreate, System.IO.FileAccess.Write));
                        writer.Write(toExport.width);
                        writer.Write(toExport.height);

                        foreach (short item in toExport.data)
                        {
                            writer.Write(item);
                        }

                        writer.Close();
                    }
                    else
                    {
                        this.GetResized((int) export.width.Value, (int) export.height.Value).GetBitmap().Save(path);
                    }
                }
            }
        }
    }
}
