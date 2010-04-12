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

namespace GeoGen_Studio
{
    public class OutputManager
    {
        public System.Drawing.Image currentImage;
        public System.Drawing.Image currentImageWithOverlay;
        public string currentImportedFile = null;
        public GGenNet.HeightData data;

        private int currentOverlayIndex;

        public OutputManager()
        {
            currentOverlayIndex = 0;
        }

        public int GetCurrentOverlayIndex()
        {
            return this.currentOverlayIndex;
        }

        public void ReloadMaps(string path)
        {
            Main main = Main.Get();
            Config config = main.GetConfig();

            main.OutputButtonsOn();

            if (path == null)
            {
                // list of generated maps
                //string[] paths = System.IO.Directory.GetFiles(config.GeoGenWorkingDirectory, "*.shd");

                //main.outputs.Items.Add("[Main]");

                // add found secondary maps to the output list
                foreach(DictionaryEntry item in main.maps)
                {
                    //System.IO.FileInfo info = new System.IO.FileInfo(paths[i]);

                    main.outputs.Items.Add((string) item.Key);
                    main.outputs3d.Items.Add((string) item.Key);
                    main.texture.Items.Add("Map: " + (string) item.Key);
                }

                main.outputs.SelectedIndex = 0;

                main.outputs3d.SelectedIndex = 0;


                if (main.currentTextureIndex < main.texture.Items.Count)
                {
                    main.texture.SelectedIndex = main.currentTextureIndex;
                }
                else
                {
                    main.texture.SelectedIndex = Main.defaultTextureIndex;
                }
            }
            else
            {
                this.currentImportedFile = path;

                main.outputs.Items.Add("[Imported File]");
                main.outputs.SelectedIndex = 0;

                main.outputs3d.Items.Add("[Imported File]");
                main.outputs3d.SelectedIndex = 0;
            }

            main.output.Left = 0;
            main.output.Top = 0;

            if (config.ActionAfterExec == Main.ActionAfterExectution.GoTo2DOutput)
            {
                main.SelectTab(Main.Tabs.Output2D);
            }
            else if (config.ActionAfterExec == Main.ActionAfterExectution.GoTo3DOutput)
            {
                main.SelectTab(Main.Tabs.Output3D);
            }

            // rebuild the 3d model
            if (config.enable3d)
            {
                main.RebuildTerrain(path);
            }
        }

        public System.Drawing.Bitmap ApplyOverlay(GGenNet.HeightData heights, System.Drawing.Bitmap overlayBitmap)
        {
            // prepare byte access to the overlay bitmap
            System.Drawing.Rectangle OverlayRect = new System.Drawing.Rectangle(0, 0, overlayBitmap.Width, overlayBitmap.Height);
            System.Drawing.Imaging.BitmapData overlayData = overlayBitmap.LockBits(OverlayRect, System.Drawing.Imaging.ImageLockMode.ReadOnly, overlayBitmap.PixelFormat);

            // create a blank bitmap and prepare it for byte access
            System.Drawing.Bitmap bitmap = new System.Drawing.Bitmap(heights.Width, heights.Height);
            System.Drawing.Rectangle rect = new System.Drawing.Rectangle(0, 0, bitmap.Width, bitmap.Height);
            System.Drawing.Imaging.BitmapData data = bitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, bitmap.PixelFormat);

            // prepare memory space for the newly created color data
            byte[] bytes = new byte[data.Stride * bitmap.Height];
            byte[] overlayCopy = new byte[overlayData.Stride * overlayBitmap.Height];

            // get a pointer to the to first line (=first pixel)
            IntPtr ptr = data.Scan0;
            IntPtr overlayPtr = overlayData.Scan0;

            // create a byte copy of the heightmap data
            System.Runtime.InteropServices.Marshal.Copy(overlayPtr, overlayCopy, 0, overlayData.Stride * overlayBitmap.Height);

            // create the color data

            // standard format overlay (positive values only)
            if (overlayBitmap.Width == 256)
            {
                for (int i = 0; i < bytes.Length; i += 4)
                {
                    int current = (heights[i / 4] / 128);

                    if (current < 0) current = 0;

                    // prevent water bleeding onto the coastline
                    if (heights[i / 4] > 0 && current == 0) current = 1;

                    bytes[i + 0] = overlayCopy[current * 3 + 0];
                    bytes[i + 1] = overlayCopy[current * 3 + 1];
                    bytes[i + 2] = overlayCopy[current * 3 + 2];
                    bytes[i + 3] = 255;
                }
            }
            // extended overlay (positive AND negative values)
            else
            {
                for (int i = 0; i < bytes.Length; i += 4)
                {
                    int current = 255 + (heights[i / 4] / 128);

                    if (current < 0 || current > 511)
                    {
                        throw new Exception("This cannot happen");
                    }
                    
                    // prevent water bleeding onto the coastline
                    if (current == 255 && heights[i / 4] > 0) current = 256;

                    bytes[i + 0] = overlayCopy[current * 3 + 0];
                    bytes[i + 1] = overlayCopy[current * 3 + 1];
                    bytes[i + 2] = overlayCopy[current * 3 + 2];
                    bytes[i + 3] = 255;
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
            Main main = Main.Get();
            Config config = main.GetConfig();

            try
            {
                main.AddStatus("Loading");

                string path = config.GeoGenWorkingDirectory + "/";

                int oldImageWidth = 0;
                int oldImageHeight = 0;

                // save original output image dimensions, so we can deetect their change
                if (main.output.Image != null)
                {
                    oldImageWidth = main.output.Image.Width;
                    oldImageHeight = main.output.Image.Height;
                }

                // load imported, main or secondary map?
                if (this.currentImportedFile != null)
                {
                    path = this.currentImportedFile;
                }
                else if (main.outputs.SelectedIndex < 1)
                {
                    path += config.MainMapOutputFile;
                }
                else
                {
                    path += (string)main.outputs.Items[main.outputs.SelectedIndex];
                }

                // if the image being loaded doesn't exist, cancel
                try
                {
                    //this.data = new SHData(path);

                    this.data = (GGenNet.HeightData)main.maps[main.outputs.SelectedItem];
                    currentImage = HeightDataToBitmap(this.data);
                }
                catch (Exception e)
                {
                    main.RemoveStatus("Loading");

                    return;
                }


                // apply overlay pattern?
                if (main.overlays.SelectedIndex > 0)
                {
                    string overlayPath = config.OverlayDirectory + "/" + (string)main.overlays.Items[main.overlays.SelectedIndex];

                    this.currentImageWithOverlay = this.ApplyOverlay(this.data, new System.Drawing.Bitmap(overlayPath));

                }

                // decide which image (gray or overlay) to display
                if (main.overlays.SelectedIndex > 0 && main.toggleOverlay.Checked)
                {
                    main.output.Image = this.currentImageWithOverlay;
                }
                else
                {
                    main.output.Image = this.currentImage;
                }

                // detect size change (reset the view if size changed to prevent the image shrinking avay from the screen)
                if (oldImageWidth > main.output.Image.Width || oldImageHeight > main.output.Image.Width || oldImageHeight == 0)
                {
                    main.output.Width = main.output.Image.Width;
                    main.output.Height = main.output.Image.Height;
                }
            }
            catch (OutOfMemoryException)
            {
                main.OutOfMemory();
            }
            
            main.RemoveStatus("Loading");
        }

        public void LoadOverlays()
        {
            Main main = Main.Get();
            Config config = main.GetConfig();

            main.overlays.Items.Clear();

            main.overlays.Items.Add("[None]");
            main.texture.Items.Add("[Import External]");

            string[] paths;

            try
            {
                paths = System.IO.Directory.GetFiles(config.OverlayDirectory, "*.bmp");
            }
            catch(Exception)
            {
                main.WriteToConsole("Could not open overlay directory.");
                return;
            }

            for (int i = 0; i < paths.Length; i++)
            {
                System.IO.FileInfo info;

                try
                {
                    info = new System.IO.FileInfo(paths[i]);

                    main.overlays.Items.Add(info.Name);
                    main.texture.Items.Add("Overlay: " + info.Name);
                }
                catch (Exception)
                {
                    continue;
                }
            }

            // try to select the overlay that was selected before (or 0 if this is loading for the first time)
            if (main.overlays.Items.Count > currentOverlayIndex)
            {
                main.overlays.SelectedIndex = currentOverlayIndex;
            }

            main.texture.SelectedIndex = main.currentTextureIndex;
        }

        public void SaveOutput()
        {
            Main main = Main.Get();
            Config config = main.GetConfig();

            if (main.FileDialog(main.saveOutputDialog, ref config.lastExportedOutput))
            {
                main.output.Image.Save(config.lastExportedOutput, System.Drawing.Imaging.ImageFormat.Png);
            }
        }

        public void ToggleOverlay()
        {
            Main main = Main.Get();

            if (main.overlays.SelectedIndex > 0 && main.toggleOverlay.Checked)
            {
                main.output.Image = this.currentImageWithOverlay;
            }
            else
            {
                main.output.Image = this.currentImage;
            }
        }

        public static System.Drawing.Bitmap HeightDataToBitmap(GGenNet.HeightData data){
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
        public static void StretchHeightValues(ref GGenNet.HeightData data){
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

        public static void ScaleHeightValues(ref GGenNet.HeightData data, short newMin, short newMax)
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

        public static GGenNet.HeightData GetResizedHeightData(GGenNet.HeightData data, int width, int height)
        {
            GGenNet.HeightData resized = new GGenNet.HeightData((UInt16)width, (UInt16)height);
            
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

        public static GGenNet.HeightData LoadHeightmapFromImageFile(string path){
            GGenNet.HeightData heights;
            
            string ext = path.Substring(path.LastIndexOf('.'), path.Length - path.LastIndexOf('.')).ToLower();

            if (ext == ".shd")
            {
                // byte-by-byte binary reading
                System.IO.BinaryReader reader = new System.IO.BinaryReader(System.IO.File.Open(path, System.IO.FileMode.Open, System.IO.FileAccess.Read));

                // read first eight bytes with map dimensions
                int width = reader.ReadInt32();
                int height = reader.ReadInt32();

                heights = new GGenNet.HeightData((UInt16) width,(UInt16) height);

                // read the double bytes containing the height data
                for (int i = 0; i < width * height; i++)
                {
                    heights[i] = reader.ReadInt16();
                }

                reader.Close();
            }
            else
            {
                // read the bitmap file
                System.Drawing.Bitmap bitmap = new System.Drawing.Bitmap(path);
                System.Drawing.Rectangle rect = new System.Drawing.Rectangle(0, 0, bitmap.Width, bitmap.Height);
                System.Drawing.Imaging.BitmapData data = bitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, bitmap.PixelFormat);

                heights = new GGenNet.HeightData((UInt16) bitmap.Width, (UInt16)bitmap.Height);

                // prepare memory space for the color data
                byte[] bytes = new byte[data.Stride * bitmap.Height];

                // get a pointer to the to first line (=first pixel)
                IntPtr ptr = data.Scan0;

                // create a byte copy of the heightmap data
                System.Runtime.InteropServices.Marshal.Copy(ptr, bytes, 0, data.Stride * bitmap.Height);

                // create the color data
                for (int i = 0; i < bytes.Length; i += 4)
                {
                    heights[i / 4] = (short)((short)bytes[i] * 128);
                }

                bitmap.UnlockBits(data);
            }

            return heights;
        }

        public void ExportData()
        {
            Main main = Main.Get();
            Config config = main.GetConfig();

            Export export = new Export();
            export.width.Maximum = main.heightData.Width;
            export.width.Value = main.heightData.Width;
            export.height.Maximum = main.heightData.Height;
            export.height.Value = main.heightData.Height;
            if (config.exportRescaleMode) export.subzeroMode2.Checked = true;

            if (export.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                if (main.FileDialog(main.exportHeightmapDialog, ref config.lastExportedFile))
                {
                    string path = main.exportHeightmapDialog.FileName;
                    string ext = path.Substring(path.LastIndexOf('.'), path.Length - path.LastIndexOf('.')).ToLower();

                    //config.lastExportedFile = path;

                    config.exportRescaleMode = export.subzeroMode2.Checked;

                    GGenNet.HeightData toExport = OutputManager.GetResizedHeightData(main.heightData, (int)export.width.Value, (int)export.height.Value);

                    // rescale the values if necessary
                    if (ext != ".shd" && export.subzeroMode2.Checked) OutputManager.ScaleHeightValues(ref toExport, 0, short.MaxValue - 1);

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
                        OutputManager.HeightDataToBitmap(toExport).Save(path);
                    }
                }
            }
        }
    }
}
