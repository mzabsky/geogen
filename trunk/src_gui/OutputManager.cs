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
using System.Text;
using System.Collections;

using GeoGen.Net;

namespace GeoGen.Studio
{
    partial class Main
    {
        public System.Drawing.Image currentImage;
        public System.Drawing.Image currentImageWithOverlay;
        public string currentImportedFile = null;
        public HeightData data;

        private int currentOverlayIndex;

        public int GetCurrentOverlayIndex()
        {
            return this.currentOverlayIndex;
        }

        public void ReloadMaps(string path)
        {
            this.OutputButtonsOn();

            if (path == null)
            {
                // list of generated maps
                //string[] paths = System.IO.Directory.GetFiles(config.GeoGenWorkingDirectory, "*.shd");

                // add found secondary maps to the output list
                foreach (DictionaryEntry item in this.maps)
                {
                    //System.IO.FileInfo info = new System.IO.FileInfo(paths[i]);

                    this.outputs.Items.Add((string)item.Key);
                    this.outputs3d.Items.Add((string)item.Key);
                    this.texture.Items.Add("Map: " + (string)item.Key);
                }

                this.outputs.SelectedIndex = 0;

                this.outputs3d.SelectedIndex = 0;

                this.ReselectTexture();

                this.currentImportedFile = null;
            }
            else
            {
                this.currentImportedFile = path;

                this.outputs.Items.Add("[Imported File]");
                this.outputs.SelectedIndex = 0;

                this.outputs3d.Items.Add("[Imported File]");
                this.outputs3d.SelectedIndex = 0;
            }

            this.output.Left = 0;
            this.output.Top = 0;

            if (config.ActionAfterExec == Main.ActionAfterExectution.GoTo2DOutput)
            {
                this.SelectTab(Main.Tabs.Output2D);
            }
            else if (config.ActionAfterExec == Main.ActionAfterExectution.GoTo3DOutput)
            {
                this.SelectTab(Main.Tabs.Output3D);
            }

            // rebuild the 3d model
            if (config.enable3d)
            {
                this.RebuildTerrain(path);
            }
        }

        public System.Drawing.Bitmap ApplyOverlay(HeightData heights, System.Drawing.Bitmap overlayBitmap)
        {
            // prepare byte access to the overlay bitmap
            System.Drawing.Rectangle OverlayRect = new System.Drawing.Rectangle(0, 0, overlayBitmap.Width, overlayBitmap.Height);
            System.Drawing.Imaging.BitmapData overlayData = overlayBitmap.LockBits(OverlayRect, System.Drawing.Imaging.ImageLockMode.ReadOnly, overlayBitmap.PixelFormat);

            // create a blank bitmap and prepare it for byte access
            System.Drawing.Bitmap bitmap = new System.Drawing.Bitmap(heights.Width, heights.Height, overlayData.PixelFormat);
            System.Drawing.Rectangle rect = new System.Drawing.Rectangle(0, 0, bitmap.Width, bitmap.Height);
            System.Drawing.Imaging.BitmapData data = bitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, overlayBitmap.PixelFormat);

            // prepare memory space for the newly created color data
            byte[] bytes = new byte[data.Stride * bitmap.Height];
            byte[] overlayCopy = new byte[overlayData.Stride * overlayBitmap.Height];

            int pixelSize = overlayData.Stride / overlayBitmap.Width;

            // get a pointer to the to first line (=first pixel)
            IntPtr ptr = data.Scan0;
            IntPtr overlayPtr = overlayData.Scan0;

            // create a byte copy of the heightmap data
            System.Runtime.InteropServices.Marshal.Copy(overlayPtr, overlayCopy, 0, overlayData.Stride * overlayBitmap.Height);

            // create the color data

            // standard format overlay (positive values only)
            if (overlayBitmap.Width == 256)
            {
                for (int y = 0; y < heights.Height; y++)
                {
                    for (int x = 0; x < heights.Width; x++)
                    {
                        int index = y * data.Stride + x * pixelSize;

                        int current = (heights[x + y * heights.Width] / 128);

                        if (current < 0) current = 0;

                        // prevent water bleeding onto the coastline
                        if (heights[x + y * heights.Width] > 0 && current == 0) current = 1;

                        for (int channelIndex = 0; channelIndex < pixelSize; channelIndex++)
                        {
                            bytes[index + channelIndex] = overlayCopy[current * pixelSize + channelIndex];
                        }
                    }
                }
            }
            // extended overlay (positive AND negative values)
            else
            {
                for (int y = 0; y < heights.Height; y++)
                {
                    for(int x = 0; x < heights.Width; x++)
                    {
                        int index = y * data.Stride + x * pixelSize;

                        int current = 255 + (heights[x + y * heights.Width] / 128);

                        if (current < 0 || current > 511)
                        {
                            throw new Exception("This cannot happen");
                        }

                        // prevent water bleeding onto the coastline
                        if (current == 255 && heights[x + y * heights.Width] > 0) current = 256;
                     
                        for (int channelIndex = 0; channelIndex < pixelSize; channelIndex++)
                        {
                            bytes[index + channelIndex] = overlayCopy[current * pixelSize + channelIndex];
                        }
                    }
                }
            }

            // copy the data into the bitmap
            System.Runtime.InteropServices.Marshal.Copy(bytes, 0, ptr, data.Stride * bitmap.Height);

            // unlock the bits
            bitmap.UnlockBits(data);

            return bitmap;
        }

        public void ShowImage()
        {
            Config config = this.GetConfig();

            try
            {
                this.AddStatus("Loading");

                int oldImageWidth = 0;
                int oldImageHeight = 0;

                // save original output image dimensions, so we can deetect their change
                if (this.output.Image != null)
                {
                    oldImageWidth = this.output.Image.Width;
                    oldImageHeight = this.output.Image.Height;
                }

                // if the image being loaded doesn't exist, cancel
                try
                {
                    // load imported or internal
                    if (this.currentImportedFile != null)
                    {
                        this.data = Main.LoadHeightmapFromImageFile(this.currentImportedFile);

                        if (this.data == null) throw new Exception();
                    }
                    else
                    {
                        this.data = (HeightData)this.maps[this.outputs.SelectedItem];
                    }
                    currentImage = HeightDataToBitmap(this.data);
                }
                catch (Exception e)
                {
                    this.RemoveStatus("Loading");

                    return;
                }


                // apply overlay pattern?
                if (this.overlays.SelectedIndex > 0)
                {
                    string overlayPath = config.OverlayDirectory + "/" + (string)this.overlays.Items[this.overlays.SelectedIndex];

                    System.Drawing.Bitmap overlay = new System.Drawing.Bitmap(overlayPath);
                    this.currentImageWithOverlay = this.ApplyOverlay(this.data, overlay);

                    overlay.Dispose();
                }

                // decide which image (gray or overlay) to display
                if (this.overlays.SelectedIndex > 0 && this.toggleOverlay.Checked)
                {
                    this.output.Image = this.currentImageWithOverlay;
                }
                else
                {
                    this.output.Image = this.currentImage;
                }

                // detect size change (reset the view if size changed to prevent the image shrinking avay from the screen)
                if (oldImageWidth > this.output.Image.Width || oldImageHeight > this.output.Image.Width || oldImageHeight == 0)
                {
                    this.output.Width = this.output.Image.Width;
                    this.output.Height = this.output.Image.Height;
                }
            }
            catch (OutOfMemoryException)
            {
                this.OutOfMemory();
            }

            this.RemoveStatus("Loading");
        }

        public void LoadOverlays()
        {
            Config config = this.GetConfig();

            this.overlays.Items.Clear();

            this.overlays.Items.Add("[None]");
            this.texture.Items.Add("[Import External]");

            string[] paths;

            try
            {
                paths = System.IO.Directory.GetFiles(config.OverlayDirectory, "*.bmp");
            }
            catch(Exception)
            {
                this.WriteToConsole("Could not open overlay directory.");
                return;
            }

            for (int i = 0; i < paths.Length; i++)
            {
                System.IO.FileInfo info;

                try
                {
                    info = new System.IO.FileInfo(paths[i]);

                    this.overlays.Items.Add(info.Name);
                    this.texture.Items.Add("Overlay: " + info.Name);
                }
                catch (Exception)
                {
                    continue;
                }
            }

            // try to select the overlay that was selected before (or 0 if this is loading for the first time)
            if (this.overlays.Items.Count > currentOverlayIndex)
            {
                this.overlays.SelectedIndex = currentOverlayIndex;
            }

            this.ReselectTexture();
        }

        public void SaveOutput()
        {
            Config config = this.GetConfig();

            if (this.FileDialog(this.saveOutputDialog, ref config.lastExportedOutput))
            {
                this.output.Image.Save(config.lastExportedOutput, System.Drawing.Imaging.ImageFormat.Png);
            }
        }

        public void ToggleOverlay()
        {
            if (this.overlays.SelectedIndex > 0 && this.toggleOverlay.Checked)
            {
                this.output.Image = this.currentImageWithOverlay;
            }
            else
            {
                this.output.Image = this.currentImage;
            }
        }

        public static System.Drawing.Bitmap HeightDataToBitmap(HeightData data){
            // create a blank bitmap
            System.Drawing.Bitmap bitmap = new System.Drawing.Bitmap(data.Width, data.Height);

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
                byte current = (byte)(data[i / 4] > 0 ? (data[i / 4] / 128) : 0);

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

        // preserves 0 level!!!
        public static void StretchHeightValues(ref HeightData data){
            // find minimum and maximum values
            short max = short.MinValue;
            short min = short.MaxValue;

            for(int i = 0; i < data.Length; i++){
                if(data[i] > max) max = data[i];
                if(data[i] < min) min = data[i];
            }

            // is the ocean deeper than land is high?
            if(max < -min) max = (short) -min;

            for (int i = 0; i < data.Length; i++){
                data[i] = max > 0 ? (short)((int)data[i] * (int)short.MaxValue / (int)max) : (short)0;
            }
        }

        public static void ScaleHeightValues(ref HeightData data, short newMin, short newMax)
        {
            // find minimum and maximum values
            int max = short.MinValue;
            int min = short.MaxValue;

            int newMaxWork = newMax - newMin;

            for (int i = 0; i < data.Length; i++)
            {
                if (data[i] > max) max = data[i];
                if (data[i] < min) min = data[i];
            }

            max -= min;

            for (int i = 0; i < data.Length; i++)
            {
                data[i] = (short)((short)newMin + (short)(((int)data[i] - min) * newMaxWork / max));
            }
        }

        public static HeightData GetResizedHeightData(HeightData data, int width, int height)
        {
            HeightData resized = new HeightData((UInt16)width, (UInt16)height);
            
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

        public static HeightData LoadHeightmapFromImageFile(string path){
            Config config = Main.Get().GetConfig();
            
            HeightData heights;
            
            string ext = path.Substring(path.LastIndexOf('.'), path.Length - path.LastIndexOf('.')).ToLower();

            if (ext == ".shd")
            {
                // byte-by-byte binary reading
                System.IO.BinaryReader reader = new System.IO.BinaryReader(System.IO.File.Open(path, System.IO.FileMode.Open, System.IO.FileAccess.Read));

                // read first eight bytes with map dimensions
                int width = reader.ReadInt32();
                int height = reader.ReadInt32();

                heights = new HeightData((UInt16) width,(UInt16) height);

                // read the double bytes containing the height data
                for (int i = 0; i < width * height; i++)
                {
                    heights[i] = reader.ReadInt16();
                }

                reader.Close();

                reader.Dispose();
            }
            else
            {
                // read the bitmap file


                System.Drawing.Bitmap bitmap;

                try
                {
                    bitmap = new System.Drawing.Bitmap(path);
                }
                catch(ArgumentException){
                    return null;
                }

                System.Drawing.Rectangle rect = new System.Drawing.Rectangle(0, 0, bitmap.Width, bitmap.Height);
                System.Drawing.Imaging.BitmapData data = bitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, bitmap.PixelFormat);

                heights = new HeightData((UInt16) bitmap.Width, (UInt16)bitmap.Height);

                // prepare memory space for the color data
                byte[] bytes = new byte[data.Stride * bitmap.Height];

                int pixelSize = data.Stride / data.Width;

                // get a pointer to the to first line (=first pixel)
                IntPtr ptr = data.Scan0;

                // create a byte copy of the heightmap data
                System.Runtime.InteropServices.Marshal.Copy(ptr, bytes, 0, data.Stride * bitmap.Height);

                // create the color data
                for (int i = 0; i < bytes.Length; i += pixelSize)
                {
                    heights[i / pixelSize] = (short)((short)bytes[i] * 128);
                }

                bitmap.UnlockBits(data);

                bitmap.Dispose();
            }

            HeightData heights2 = Main.GetResizedHeightData(heights, Math.Min(heights.Width, (int)config.mapDetailLevel), Math.Min(heights.Height, (int)config.mapDetailLevel));

            return heights2;
        }

        public void ExportData()
        {
            Config config = this.GetConfig();

            Export export = new Export();
            export.width.Maximum = this.heightData.Width;
            export.width.Value = this.heightData.Width;
            export.height.Maximum = this.heightData.Height;
            export.height.Value = this.heightData.Height;
            if (config.exportRescaleMode) export.subzeroMode2.Checked = true;

            if (export.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                if (this.FileDialog(this.exportHeightmapDialog, ref config.lastExportedFile))
                {
                    string path = this.exportHeightmapDialog.FileName;
                    string ext = path.Substring(path.LastIndexOf('.'), path.Length - path.LastIndexOf('.')).ToLower();

                    //config.lastExportedFile = path;

                    config.exportRescaleMode = export.subzeroMode2.Checked;

                    HeightData toExport = Main.GetResizedHeightData(this.heightData, (int)export.width.Value, (int)export.height.Value);

                    // rescale the values if necessary
                    if (ext != ".shd" && export.subzeroMode2.Checked) Main.ScaleHeightValues(ref toExport, 0, short.MaxValue - 1);

                    if (ext == ".shd")
                    {
                        System.IO.BinaryWriter writer = new System.IO.BinaryWriter(System.IO.File.Open(path, System.IO.FileMode.OpenOrCreate, System.IO.FileAccess.Write));
                        writer.Write(toExport.Width);
                        writer.Write(toExport.Height);

                        for (int i = 0; i < toExport.Length; i++ )
                        {
                            writer.Write(toExport[i]);
                        }

                        writer.Close();
                    }
                    else
                    {
                        Main.HeightDataToBitmap(toExport).Save(path);
                    }

                    toExport.Dispose();
                }                               
            }
        }
    }
}
