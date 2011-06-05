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
using System.Windows.Forms;

using OpenTK;
using OpenTK.Graphics;
using OpenTK.Graphics.OpenGL;

using System.Runtime.InteropServices;

using GeoGen.Net;

namespace GeoGen.Studio
{
    public partial class Main
    {
        public enum ModelDetailLevel
        {
            VeryLow_128x128Polygons = 128,
            Low_256x256Polygons = 256,
            Medium_512x512Polygons = 512,
            High_1024x1024Polygons = 1024,
            VeryHigh_2048x2048Polygons = 2048/*,
            Extreme_4096x4096Polygons = 4096*/
        };

        public enum BitmapDetailLevel
        {
            VeryLow_128x128Pixels = 128,
            Low_256x256Pixels = 256,
            Medium_512x512Pixels = 512,
            High_1024x1024Pixels = 1024,
            VeryHigh_2048x2048Pixels = 2048,
            Extreme_4096x4096Pixels = 4096
        };

        public enum ViewportBackground
        {
            Black = 0,
            DarkGray = 63,
            MediumGray = 127,
            LightGray = 190,
            White = 255
        }

        public enum TextureScalingAlgorithm
        {
            NearestNeighbor,
            LinearInterpolation
        }

        [StructLayout(LayoutKind.Sequential)]
        struct Vertex
        { // mimic InterleavedArrayFormat.T2fN3fV3f
            public Vector2 TexCoord;
            public Vector3 Normal;
            public Vector3 Position;

            // copy constructor
            public Vertex(Vertex v)
            {
                this.TexCoord = new Vector2(v.TexCoord);
                this.Normal = new Vector3(v.Normal);
                this.Position = new Vector3(v.Position);
            }
        }

        public static string defaultTexture = "Overlay: TopoBathy.bmp";

        public HeightData heightData;

        public System.Threading.Thread modelThread;

        public double azimuth = 0.785398; // 45 degrees
        public double elevation = 0.52; // 30 degrees
        public float distance = 100;
        public float targetX = 50; // default position is square center
        public float targetY = 50;
        public int currentMap = -1;
        public string currentTexture = defaultTexture;

        private int vertexBufferHandle;
        private int indexBufferHandle;
        private int textureHandle;

        private bool ready3d = false;

        //private float heightScale = 8f;

        private HeightData textureBase;
        /*
        public float HeightScale
        {
            get { return heightScale; }
            set { 
                
                heightScale = value;

                // the viewport might not exist yet
                try
                {
                    
                }
                catch (Exception) { };
            }
        }*/

        public void Init()
        {
            this.SetupViewport();
        }

        public void ReselectTexture(){
            int index = this.texture.Items.IndexOf(this.currentTexture);
            if(index != -1){
                this.texture.SelectedIndex = index;
            }
            else {
                this.texture.SelectedIndex = Math.Max(0, this.texture.Items.IndexOf(Main.defaultTexture));
            }
        }

        public void SetupViewport(){
            Config config = Main.Get().GetConfig();

            Matrix4 projection = Matrix4.CreatePerspectiveFieldOfView((float)Math.PI / 4, this.viewport.Width / (float)this.viewport.Height, 0.1f, 3000.0f);
            //Matrix4 projection = Matrix4.CreateOrthographic(120,  (float)this.viewport.Height / (float)this.viewport.Width * 120f, 0.1f, 3000.0f);



            GL.MatrixMode(MatrixMode.Projection);
            GL.LoadMatrix(ref projection);
            GL.Enable(EnableCap.DepthTest);
            GL.Enable(EnableCap.Texture2D);
            GL.Enable(EnableCap.Multisample);
            GL.Enable(EnableCap.RescaleNormal);
            GL.Enable(EnableCap.Normalize);
            //GL.Enable(EnableCap.CullFace);
            GL.Viewport(0, 0, this.viewport.Width, this.viewport.Height);

            GL.Enable(EnableCap.Lighting);
            GL.Enable(EnableCap.Light0);
            GL.Enable(EnableCap.ColorMaterial);
            GL.ColorMaterial(MaterialFace.Front, ColorMaterialParameter.AmbientAndDiffuse);
            GL.ShadeModel(ShadingModel.Smooth);

            GL.EnableClientState(EnableCap.VertexArray);
            GL.EnableClientState(EnableCap.TextureCoordArray);
            GL.EnableClientState(EnableCap.NormalArray);



            GL.ClearColor((float) config.BackgroundColor3d / 255, (float) config.BackgroundColor3d / 255, (float) config.BackgroundColor3d / 255, 1.0f);

            //viewport.Invalidate();
        }

        public void ClearData3D()
        {
            this.Output3dButtonsOff();

            // terminate the model calculation worker thread
            if (this.modelThread != null)
            {
                this.modelThread.Abort();

                this.HideBuildingModel();
            }

            // release the height data
            if(this.heightData != null) this.heightData.Dispose();
            if (this.textureBase != null) this.textureBase.Dispose();
            this.heightData = null;
            this.textureBase = null;

            this.outputs3d.Items.Clear();

            // remove "Maps:" entries from the texture list
            for (int i = 0; i < this.texture.Items.Count; i++)
            {
                char c = ((string)this.texture.Items[i])[0];

                if (((string)this.texture.Items[i])[0] == 'M')
                {
                    this.texture.Items.RemoveAt(i);
                    
                    // the indexes got shifted by deleting the current item
                    i--;
                }
            }

            // free Video-RAM
            if (this.vertexBufferHandle != 0)
            {
                GL.DeleteBuffers(1, ref this.vertexBufferHandle);
                GL.DeleteBuffers(1, ref this.indexBufferHandle);
            }

            if (this.textureHandle != 0)
            {
                GL.DeleteTexture(this.textureHandle);
            }

            this.vertexBufferHandle = 0;
            this.textureHandle = 0;

            // let the viewport show empty screen
            this.viewport.Invalidate();
        }

        public void SetWireframeState(bool wireframe){
            // polygon mode
            if (!wireframe)
            {
                GL.PolygonMode(MaterialFace.FrontAndBack, PolygonMode.Fill);
            }
            // wireframe mode
            else
            {
                GL.PolygonMode(MaterialFace.FrontAndBack, PolygonMode.Line);
            }

            this.viewport.Invalidate();
        }

        public void RebuildTerrain(string path_override)
        {
            Config config = this.GetConfig();

            HeightData data;

            this.currentMap = this.outputs3d.SelectedIndex;

            if (this.currentMap == -1) return;

            if (path_override == null)
            {
                data = (HeightData)this.maps[this.outputs3d.SelectedItem];
            }
            else
            {
                data = Main.LoadHeightmapFromImageFile(path_override);

                /* The map was not loaded */
                if (data == null)
                {
                    System.Windows.Forms.MessageBox.Show("Could not load the image!");
                    this.OutputButtonsOff();
                    return;
                }
            }

            

            this.ShowBuildingModel();

            System.Threading.ThreadStart starter = delegate { this.SetTerrain(data); };
            this.modelThread = new System.Threading.Thread(starter);
            this.modelThread.Start();
        }

        Vector3 CalculateNormal(Vector3 a, Vector3 b, Vector3 c)
        {
            float dX1 = a.X - b.X;
            float dY1 = a.Y - b.Y;
            float dZ1 = a.Z - b.Z;

            float dX2 = b.X - c.X;
            float dY2 = b.Y - c.Y;
            float dZ2 = b.Z - c.Z;
         
            Vector3 normal = new Vector3();

            normal.X = (dY1 * dZ2) - (dZ1 * dY2);
            normal.Y = (dZ1 * dX2) - (dX1 * dZ2);
            normal.Z = (dX1 * dY2) - (dY1 * dX2);

            normal.Normalize();

            return  normal;
        }


        public void SetTerrain(HeightData original){
            Config config = this.GetConfig();

            try
            {
                // store original's size
                int originalHeight = original.Height;
                int originalWidth = original.Width;
                // load the overlay pattern
                //System.Drawing.Bitmap overlayBitmap = new System.Drawing.Bitmap("../overlays/Topographic.bmp");

                // prepare memory space for the newly created color data
                this.heightData = Main.GetResizedHeightData(original, Math.Min(original.Width, (int)config.ModelDetailLevel), Math.Min(original.Height, (int)config.ModelDetailLevel));

                this.textureBase = Main.GetResizedHeightData(original, Math.Min(original.Width, (int)config.TextureDetailLevel), Math.Min(original.Height, (int)config.TextureDetailLevel));

                // release some memory (to prevent OutOfMemory exception)
              // original = null;

                // dimension multipliers
                float fWidth = 100f / (float)this.heightData.Width;
                float fHeight = 100f / (float)this.heightData.Height;
                float texFWidth = fWidth;
                float texFHeight = fHeight;
                float offsetX = 0;
                float offsetY = 0;

                // adjust the multipliers for non-square bitmaps
                if (originalHeight > originalWidth)
                {
                    offsetY = (float)((float)(this.heightData.Height - this.heightData.Width) * 100f / (float)this.heightData.Height) / 2f;
                    fWidth *= (float)originalWidth / (float)originalHeight;
                }
                else if (originalHeight < originalWidth)
                {
                    offsetY = (float)((float)(this.heightData.Width - this.heightData.Height) * 100f / (float)this.heightData.Width) / 2f;
                    fHeight *= (float)originalHeight / (float)originalWidth;
                }

                   
                // the vertex array for the model
                Vertex[] vertices = new Vertex[this.heightData.Length ];

                // fill in vertex data (only position and texcoords for now)
                for (int y = 0; y < this.heightData.Height; y++)
                {
                    float fy = (float)y;

                    // precalculate some stuff that stays constant for whole row
                    float yPos = offsetY + (fy + 0.5f) * fHeight;
                    float texYPos = (fy + 0.5f) * texFHeight;

                    for (int x = 0; x < this.heightData.Width; x++)
                    {
                        float fx = (float)x;

                        int vertexIndex = x + y * this.heightData.Width;

                        vertices[vertexIndex].Position.X = offsetX + fx * fWidth;
                        vertices[vertexIndex].Position.Y = yPos;
                        vertices[vertexIndex].Position.Z = (float)((float)this.heightData[x, y] * 0.005f / 128f);

                        if (!this.config.enableTerrainUnderZero && vertices[vertexIndex].Position.Z < 0) vertices[vertexIndex].Position.Z = 0;

                        vertices[vertexIndex].TexCoord.X = (fx + 0.5f) * texFWidth / 100f;
                        vertices[vertexIndex].TexCoord.Y = texYPos / 100f;
                    }
                }

                uint[] indices = new uint[this.heightData.Length * 6];

                // build index array
                for (int y = 0; y < this.heightData.Height - 1; y++)
                {
                    for (int x = 0; x < this.heightData.Width - 1; x++)
                    {
                        float fx = (float)x;

                        int vertexIndex = (x + y * this.heightData.Width) * 6;

                        // first triangle
                        indices[vertexIndex] = (uint)(x + y * this.heightData.Width);
                        indices[vertexIndex + 1] = (uint)(x + 1 + y * this.heightData.Width);
                        indices[vertexIndex + 2] = (uint)(x + (y + 1) * this.heightData.Width);

                        // second triangle                        
                        indices[vertexIndex + 3] = (uint)(x + 1 + (y + 1) * this.heightData.Width);
                        indices[vertexIndex + 4] = (uint)(x + (y + 1) * this.heightData.Width);
                        indices[vertexIndex + 5] = (uint)(x + 1 + y * this.heightData.Width);
                    }
                }

                // build face normals
                Vector3[] faceNormals = new Vector3[(this.heightData.Width - 1) * (this.heightData.Height - 1) * 2];

                for (int y = 0; y < this.heightData.Height - 1; y++)
                {
                    for (int x = 0; x < this.heightData.Width - 1; x++)
                    {
                        faceNormals[(x + y * (this.heightData.Width - 1)) * 2] = this.CalculateNormal(vertices[x + (y + 1) * this.heightData.Width].Position, vertices[x + y * this.heightData.Width].Position, vertices[(x + 1) + y * this.heightData.Width].Position);
                        faceNormals[(x + y * (this.heightData.Width - 1)) * 2 + 1] = this.CalculateNormal(vertices[x + (y + 1) * this.heightData.Width].Position, vertices[x + y * this.heightData.Width].Position, vertices[(x + 1) + (y + 1) * this.heightData.Width].Position);
                    }
                }

                // build vertex normals
                for (int y = 0; y < this.heightData.Height; y++)
                {
                    for (int x = 0; x < this.heightData.Width; x++)
                    {
                        int faceCount = 0;

                        // upper left triangle
                        if (x > 0 && y > 0)
                        {
                            vertices[x + y * this.heightData.Width].Normal = faceNormals[((x - 1) + (y - 1) * (this.heightData.Width - 1)) * 2 + 1] ;

                            faceCount++;
                        }

                        // bottom left triangles
                        if (x > 0 && y < this.heightData.Height - 1)
                        {
                            vertices[x + y * this.heightData.Width].Normal += faceNormals[((x - 1) + y * (this.heightData.Width - 1)) * 2];
                            vertices[x + y * this.heightData.Width].Normal += faceNormals[((x - 1) + y * (this.heightData.Width - 1)) * 2 + 1];

                            faceCount += 2;
                        }

                        // upper right triangles
                        if (x < this.heightData.Width - 1 && y > 0)
                        {
                            vertices[x + y * this.heightData.Width].Normal += faceNormals[(x + (y - 1) * (this.heightData.Width - 1)) * 2];
                            vertices[x + y * this.heightData.Width].Normal += faceNormals[(x + (y - 1) * (this.heightData.Width - 1)) * 2 + 1];

                            faceCount += 2;
                        }

                        if (x < this.heightData.Width - 1 && y < this.heightData.Height - 1)
                        {
                            vertices[x + y * this.heightData.Width].Normal += faceNormals[(x + y * (this.heightData.Width - 1)) * 2];

                            faceCount++;
                        }

                        vertices[x + y * this.heightData.Width].Normal /= faceCount;
                    }
                }

                // release the context from the GUI thread
                this.Invoke(new System.Windows.Forms.MethodInvoker(delegate()
                {
                    viewport.Context.MakeCurrent(null);
                }));

                // grab the context for this thread
                viewport.MakeCurrent();

                // delete the previous buffer content
                if (this.vertexBufferHandle != 0)
                {
                    GL.DeleteBuffers(1, ref this.vertexBufferHandle);
                }

                // allocate the buffer
                GL.GenBuffers(1, out this.vertexBufferHandle);
                GL.GenBuffers(1, out this.indexBufferHandle);

                // tell that we are using that buffer
                GL.BindBuffer(BufferTarget.ArrayBuffer, this.vertexBufferHandle);

                GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(vertices.Length * 8 * sizeof(float)), vertices, BufferUsageHint.StaticDraw);
                
                GL.BindBuffer(BufferTarget.ElementArrayBuffer, this.indexBufferHandle);

                // upload the data into the buffer into GPU
                GL.BufferData(BufferTarget.ElementArrayBuffer, (IntPtr)(indices.Length * sizeof(uint)), indices, BufferUsageHint.StaticDraw);


                // make sure the massive vertex array is gone from RAM
                vertices = null;
                indices = null;

                // release the context from current thread
                viewport.Context.MakeCurrent(null);

                try
                {
                    this.Invoke(new System.Windows.Forms.MethodInvoker(delegate()
                    {
                        try
                        {
                            // regrab the context for the GUI thread
                            viewport.MakeCurrent();

                            // rebuild the texture
                            this.ApplyTexture();

                            // UI stuff
                            this.Output3dButtonsOn();
                            this.viewport.Invalidate();

                            this.HideBuildingModel();
                        }
                        catch (Exception e)
                        {

                        }
                    }));
                }
                // this might throw exceptions in case the main thread was terminated while this thread is running
                catch (Exception e) { 
                
                };
            }
            catch (OutOfMemoryException)
            {
                try{
                    this.Invoke(new System.Windows.Forms.MethodInvoker(delegate()
                    {
                        this.HideBuildingModel();

                        this.OutOfMemory();
                    }));
                }
                catch{
                    return;
                };
            }
        }

        public void Render(PaintEventArgs e)
        {
            if (!this.ready3d || !this.config.enable3d)
            {
                // the OpenGL context is not ready (for it is most likely being worked on in another
                // thread) -> manually fill the control area with flat bakground
                System.Drawing.Brush brush = new System.Drawing.SolidBrush(System.Drawing.Color.FromArgb((int)config.BackgroundColor3d, (int)config.BackgroundColor3d, (int)config.BackgroundColor3d));

                e.Graphics.FillRectangle(brush, 0, 0, viewport.Width, viewport.Height);

                return;
            }

            GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);

            float sina = (float) Math.Sin(azimuth);
            float cosa = (float)Math.Cos(azimuth);
            float cose = (float)Math.Cos(elevation);

            // setup camera
            Matrix4 modelview = Matrix4.LookAt(this.distance * cose * sina + this.targetX, this.distance * cose * cosa + this.targetY, this.distance * (float)Math.Sin(this.elevation), this.targetX, this.targetY, 0, 0, 0, 1);
            GL.MatrixMode(MatrixMode.Modelview);
            GL.LoadMatrix(ref modelview);
            
            // lighting
            GL.Light(LightName.Light0, LightParameter.Ambient, new Vector4(0.2f, 0.2f, 0.2f, 1));
            GL.Light(LightName.Light0, LightParameter.Diffuse, new Vector4(0.6f, 0.6f, 0.6f, 1));
            GL.Light(LightName.Light0, LightParameter.Specular, new Vector4(0, 0, 0, 0));

            // tell the OpenGL which buffer are we using
            GL.BindBuffer(BufferTarget.ArrayBuffer, this.vertexBufferHandle);
            GL.BindBuffer(BufferTarget.ElementArrayBuffer, this.indexBufferHandle);
            GL.BindTexture(TextureTarget.ProxyTexture2D, this.textureHandle);
            
            // tell the format of the buffered data
            GL.TexCoordPointer(2, TexCoordPointerType.Float, 8 * sizeof(float), (IntPtr)(0));
            GL.NormalPointer(NormalPointerType.Float, 8 * sizeof(float), (IntPtr)(2 * sizeof(float)));
            GL.VertexPointer(3, VertexPointerType.Float, 8 * sizeof(float), (IntPtr)(5 * sizeof(float)));

            int scalingMode = (int)(config.TextureScalingAlgorithm == TextureScalingAlgorithm.LinearInterpolation ? TextureMagFilter.Linear : TextureMagFilter.Nearest);
            GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, scalingMode);
            GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)TextureMagFilter.Linear);
            GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapS, (int) TextureWrapMode.Clamp);
            GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapR, (int)TextureWrapMode.Clamp);
            GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapT, (int)TextureWrapMode.Clamp);

            // how to vertically scale the data
            GL.Scale(1f, 1f, (float) this.heightScale.Value);

            // read the data from buffer
            GL.DrawElements(BeginMode.Triangles, (int) (this.heightData.Length * 6),DrawElementsType.UnsignedInt, IntPtr.Zero);

            // display the stuff
            viewport.SwapBuffers();
        }

        public void ApplyTexture()
        {
            Config config = this.GetConfig();

            if (this.textureBase == null) return;

                try{
                if (this.textureHandle != 0)
                {
                    GL.DeleteTexture(this.textureHandle);
                }

                string selected = (string)this.texture.Items[this.texture.SelectedIndex];
                     
                string path = "";

                System.Drawing.Bitmap bitmap = null;

                if (selected == "[Import External]")
                {
                    try{
                        if (this.FileDialog(this.importTextureDialog, ref config.lastImportedTexture))
                        {
                            bitmap = new System.Drawing.Bitmap(config.lastImportedTexture);

                            if(bitmap.Width > 4096 || bitmap.Height > 4096)
                            {
                                MessageBox.Show("The texture is too big.");
                                bitmap.Dispose();
                                this.viewport.Invalidate();
                                return;
                            }
                        }
                        else
                        {
                            this.viewport.Invalidate();
                            return;
                        }
                    }
                    catch{
                        System.Windows.Forms.MessageBox.Show("Could not load external texture.");

                        this.viewport.Invalidate();
                        return;
                    }
                }

                // "Overlay: " type texture
                else if (selected[0] == 'O')
                {
                    path = config.overlayDirectory + "/" + selected.Substring(9, selected.Length - 9);
                    
                    System.Drawing.Bitmap overlay = new System.Drawing.Bitmap(path);
                    bitmap = this.ApplyOverlay(this.textureBase, overlay);

                    overlay.Dispose();
                }

                // "Map: " type texture
                else if (selected[0] == 'M')
                {
                    bitmap = Main.HeightDataToBitmap((HeightData)this.maps[selected.Substring(5, selected.Length - 5)]);
                }

                if(config.EnableBlackCompensation) Main.ApplyBlackCompensation(ref bitmap);

                System.Drawing.Rectangle rect = new System.Drawing.Rectangle(0, 0, bitmap.Width, bitmap.Height);

                System.Drawing.Bitmap convertedBitmap = new System.Drawing.Bitmap(bitmap.Width, bitmap.Height, System.Drawing.Imaging.PixelFormat.Format32bppRgb);
                using (System.Drawing.Graphics gr = System.Drawing.Graphics.FromImage(convertedBitmap))
                {
                    gr.DrawImage(bitmap, rect);
                }

                System.Drawing.Imaging.BitmapData data = bitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, System.Drawing.Imaging.PixelFormat.Format32bppArgb);

                this.textureHandle = GL.GenTexture();
                GL.BindTexture(TextureTarget.Texture2D, this.textureHandle);

                GL.TexImage2D(TextureTarget.Texture2D, 0, PixelInternalFormat.Three, data.Width, data.Height, 0,
                OpenTK.Graphics.OpenGL.PixelFormat.Bgra, PixelType.UnsignedByte, data.Scan0);

                bitmap.UnlockBits(data);
            }
            catch (OutOfMemoryException)
            {
                this.OutOfMemory();
            }

            this.viewport.Invalidate();
        }

        public void SaveScreenshot()
        {
            Config config = this.GetConfig();

            if (this.FileDialog(this.saveOutputDialog, ref config.lastImportedTexture))
            {
                this.viewport.GrabScreenshot().Save(config.lastImportedTexture, System.Drawing.Imaging.ImageFormat.Png);
            }
        }

        private static void ApplyBlackCompensation(ref System.Drawing.Bitmap bitmap)
        {
            // create a blank bitmap and prepare it for byte access
            System.Drawing.Rectangle rect = new System.Drawing.Rectangle(0, 0, bitmap.Width, bitmap.Height);
            System.Drawing.Imaging.BitmapData data = bitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, bitmap.PixelFormat);

            int pixelSize = data.Stride / data.Width;

            // prepare memory space for the newly created color data
            byte[] bytes = new byte[data.Stride * bitmap.Height];

            // get a pointer to the to first line (=first pixel)
            IntPtr ptr = data.Scan0;
            
            // copy the data for direct access
            System.Runtime.InteropServices.Marshal.Copy(ptr, bytes, 0, data.Stride * bitmap.Height);

            // apply the compensation
            for (int y = 0; y < bitmap.Height; y++)
            {
                for (int x = 0; x < bitmap.Width; x++)
                {
                    int index = y * data.Stride + x * pixelSize;

                    for (int channelIndex = 0; channelIndex < pixelSize; channelIndex++)
                    {
                        bytes[index + channelIndex] = bytes[index + channelIndex] > (byte)64 ? bytes[index + channelIndex] : (byte)64;
                    }                    
                }
            }

            // copy the data into the bitmap
            System.Runtime.InteropServices.Marshal.Copy(bytes, 0, ptr, data.Stride * bitmap.Height);

            // unlock the bits
            bitmap.UnlockBits(data);
        }
    }
}
