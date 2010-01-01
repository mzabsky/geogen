using System;
using System.Collections.Generic;
using System.Text;

namespace GeoGen_Studio
{
    public class OutputManager
    {
        private System.Drawing.Image currentImage;
        private System.Drawing.Image currentImageWithOverlay;

        private int currentOverlayIndex;

        public OutputManager()
        {
            currentOverlayIndex = 0;
        }

        public int GetCurrentOverlayIndex()
        {
            return this.currentOverlayIndex;
        }

        public void ClearData()
        {
            Main main = Main.Get();
            Config config = main.GetConfig();

            main.OutputButtonsOff();

            if (main.output.Image != null)
            {
                // empty the output list
                main.outputs.Items.Clear();

                // free the pointers (so garbace collector can do its job)
                //images = null;
               // imagesWithOverlay = null;
                main.output.Image = null;
                currentImage = null;
                currentImageWithOverlay = null;

                // collect the garbage (there are now possibly hundreds of megabytes of garbage laying around by now)
                System.GC.Collect();

                // reset the image size (so it doesn't show error image)
                main.output.Width = 0;
                main.output.Height = 0;
            }

            // make sure the intermediate data saved on the hard drive are gone as well
            try
            {
                System.IO.File.Delete(config.GeoGenWorkingDirectory + "/" + config.MainMapOutputFile);
                if (System.IO.Directory.Exists(config.GeoGenWorkingDirectory))
                {
                    System.IO.Directory.Delete(config.GeoGenWorkingDirectory, true);
                }
            }
            // do no bug if some of the stuff being deleted is not present
            catch (System.IO.DirectoryNotFoundException) { }
            catch (System.IO.FileNotFoundException) { }
            catch (System.IO.IOException) { };

            // recreate the directory
            System.IO.Directory.CreateDirectory(config.GeoGenWorkingDirectory);
        }

        public void CaptureOutputs()
        {
            Main main = Main.Get();
            Config config = main.GetConfig();

            main.OutputButtonsOn();

            string[] paths = System.IO.Directory.GetFiles(config.GeoGenWorkingDirectory, "*.bmp");

            main.outputs.Items.Add("[Main]");
            main.outputs.SelectedIndex = 0;

            for (int i = 0; i < paths.Length; i++)
            {
                System.IO.FileInfo info = new System.IO.FileInfo(paths[i]);

                main.outputs.Items.Add(info.Name);
            }

            /*if (main.overlays.SelectedIndex != 0)
            {

            }*/

            this.ShowImage();

            main.output.Left = 0;
            main.output.Top = 0;

            if (config.GoToOutputViewAfterExec)
            {
                main.SelectTab(Main.Tabs.Output2D);
            }
        }

        public void ShowImage()
        {
            Main main = Main.Get();
            Config config = main.GetConfig();

            main.AddStatus("Loading");

            string path = config.GeoGenWorkingDirectory + "/";

            int oldImageWidth = 0;
            int oldImageHeight = 0;

            if (main.output.Image != null)
            {
                oldImageWidth = main.output.Image.Width;
                oldImageHeight = main.output.Image.Height;
            }


            if (main.outputs.SelectedIndex < 1)
            {
                path += config.MainMapOutputFile;
            }
            else
            {
                path += (string)main.outputs.Items[main.outputs.SelectedIndex];
            }

            try
            {
                currentImage = System.Drawing.Image.FromFile(path);
            }
            catch (Exception)
            {
                return;
            }

            if (main.overlays.SelectedIndex > 0)
            {
                string overlayPath = config.OverlayDirectory + "/" + (string)main.overlays.Items[main.overlays.SelectedIndex];

                // prepare byte access to the overlay bitmap
                System.Drawing.Bitmap overlayBitmap = new System.Drawing.Bitmap(overlayPath);
                System.Drawing.Rectangle OverlayRect = new System.Drawing.Rectangle(0, 0, overlayBitmap.Width, overlayBitmap.Height);
                System.Drawing.Imaging.BitmapData overlayData = overlayBitmap.LockBits(OverlayRect, System.Drawing.Imaging.ImageLockMode.ReadOnly, overlayBitmap.PixelFormat);

                // prepare byte access to the height data
                System.Drawing.Bitmap bitmap = new System.Drawing.Bitmap(currentImage);
                System.Drawing.Rectangle rect = new System.Drawing.Rectangle(0, 0, bitmap.Width, bitmap.Height);
                System.Drawing.Imaging.BitmapData data = bitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, bitmap.PixelFormat);

                // prepare memory space for the newly created color data
                byte[] copy = new byte[data.Stride * bitmap.Height];
                byte[] overlayCopy = new byte[overlayData.Stride * overlayBitmap.Height];

                // get a pointer to the to first line (=first pixel)
                IntPtr ptr = data.Scan0;
                IntPtr overlayPtr = overlayData.Scan0;

                // create a byte copy of the heightmap data
                System.Runtime.InteropServices.Marshal.Copy(ptr, copy, 0, data.Stride * bitmap.Height);
                System.Runtime.InteropServices.Marshal.Copy(overlayPtr, overlayCopy, 0, overlayData.Stride * overlayBitmap.Height);

                // apply the recoloring
                for (int i = 0; i < copy.Length; i += 4)
                {
                    copy[i + 0] = overlayCopy[copy[i + 0] * 3 + 0];
                    copy[i + 1] = overlayCopy[copy[i + 1] * 3 + 1];
                    copy[i + 2] = overlayCopy[copy[i + 2] * 3 + 2];
                    // we are not interested in alpha channel
                }

                // copy the data back
                System.Runtime.InteropServices.Marshal.Copy(copy, 0, ptr, data.Stride * bitmap.Height);

                // unlock the bits
                bitmap.UnlockBits(data);

                this.currentImageWithOverlay = bitmap;

            }

            if (main.overlays.SelectedIndex > 0 && main.toggleOverlay.Checked)
            {
                main.output.Image = this.currentImageWithOverlay;
            }
            else
            {
                main.output.Image = this.currentImage;
            }

            if (oldImageWidth != main.output.Image.Width || oldImageHeight != main.output.Image.Width)
            {
                main.output.Width = main.output.Image.Width;
                main.output.Height = main.output.Image.Height;
            }
            
            main.RemoveStatus("Loading");
        }

        public void LoadOverlays()
        {
            Main main = Main.Get();
            Config config = main.GetConfig();

            main.overlays.Items.Clear();

            main.overlays.Items.Add("[None]");

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
        }

        public void SaveOutput()
        {
            Main main = Main.Get();

            if (main.saveOutputDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                main.output.Image.Save(main.saveOutputDialog.FileName, System.Drawing.Imaging.ImageFormat.Png);
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
    }
}
