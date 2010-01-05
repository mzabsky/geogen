using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using OpenTK;
using OpenTK.Graphics;
using OpenTK.Graphics.OpenGL;

using System.Runtime.InteropServices;

namespace GeoGen_Studio
{
    public class ViewportManager
    {
        public enum ModelDetailLevel
        {
            VeryLow_128x128Polygons = 128,
            Low_256x256Polygons = 256,
            Medium_512x512Polygons = 512,
            High_1024x1024Polygons = 1024,
            VeryHigh_2048x2048Polygons = 2048,
            Extreme_4096x4096Plygons = 4096
        };

        [StructLayout(LayoutKind.Sequential)]
        struct Vertex
        { // mimic InterleavedArrayFormat.C3fV3f
            public Vector2 TexCoord;
            public Vector3 Position;
        }

        public OpenTK.GLControl viewport;
        public byte[] heightData;
        private int terrainHeight;
        private int terrainWidth;

        public System.Threading.Thread modelThread;

        public double azimuth = 0.785398; // 45 degrees
        public double elevation = 0.52; // 30 degrees
        public float distance = 100;
        public float targetX = 50; // default position is square center
        public float targetY = 50;
        public int currentMap = -1;
        public int currentTextureIndex = 6;

        private int vertexBufferHandle;
        private int textureHandle;

        private float heightScale = 8f;

        private System.Drawing.Bitmap textureBase;

        public float HeightScale
        {
            get { return heightScale; }
            set { 
                
                heightScale = value;

                // the viewport might not exist yet
                try
                {
                    this.viewport.Invalidate();
                }
                catch (Exception) { };
            }
        }

        public void Init()
        {
            Matrix4 projection = Matrix4.CreatePerspectiveFieldOfView((float)Math.PI / 4, this.viewport.Width / (float)this.viewport.Height, 1.0f, 3000.0f);
            GL.MatrixMode(MatrixMode.Projection);
            GL.LoadMatrix(ref projection);
            GL.Enable(EnableCap.DepthTest);
            GL.Enable(EnableCap.Texture2D);
            GL.Viewport(0, 0, this.viewport.Width, this.viewport.Height); // Use all of the glControl painting are

            //GL.Enable(EnableCap.Lighting);
            GL.Enable(EnableCap.Light0);
            GL.Enable(EnableCap.ColorMaterial);
            GL.ColorMaterial(MaterialFace.FrontAndBack, ColorMaterialParameter.AmbientAndDiffuse);
            //GL.ShadeModel(ShadingModel.Smooth);
            
            //GL.EnableClientState(EnableCap.ColorArray);

            GL.EnableClientState(EnableCap.VertexArray);
            GL.EnableClientState(EnableCap.TextureCoordArray);

            
  
        }

        public void SetupViewport(){

        }

        public void ClearData()
        {
            Main main = Main.Get();

            main.Output3dButtonsOff();

            // terminate the model calculation worker thread
            if (this.modelThread != null)
            {
                this.modelThread.Abort();

                main.HideBuildingModel();
            }

            // release the height data
            this.heightData = null;
            this.terrainHeight = 0;
            this.terrainWidth = 0;

            main.outputs3d.Items.Clear();

            // remove "Maps:" entrie from the texture list
            for (int i = 0; i < main.texture.Items.Count; i++)
            {
                if (((string)main.texture.Items[i])[0] == 'M')
                {
                    main.texture.Items.RemoveAt(i);
                }
            }

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

        public void RebuildTerrain()
        {
            Main main = Main.Get();
            Config config = main.GetConfig();

            this.currentMap = main.outputs3d.SelectedIndex;

            string path = config.GeoGenWorkingDirectory + "/";

            // load main or secondary map?
            if (main.outputs3d.SelectedIndex < 1)
            {
                path += config.MainMapOutputFile;
            }
            else
            {
                path += (string)main.outputs3d.Items[main.outputs3d.SelectedIndex];
            }

            main.ShowBuildingModel();

            System.Threading.ThreadStart starter = delegate { this.SetTerrain(path); };
            this.modelThread = new System.Threading.Thread(starter);
            this.modelThread.Start();
        }

        public void SetTerrain(string path){
            Main main = Main.Get();
            Config config = main.GetConfig();

            // the original map (as generated)
            System.Drawing.Bitmap original = new System.Drawing.Bitmap(path);

            // store original's size
            int originalHeight = original.Height;
            int originalWidth = original.Width;

            // resized bitmap (from which the model will be generated 1:1)
            System.Drawing.Bitmap bitmap = new System.Drawing.Bitmap(original, Math.Min(original.Width, (int)config.ModelDetailLevel), Math.Min(original.Height, (int)config.ModelDetailLevel));

            this.textureBase = bitmap;

            // prepare byte access to the height data
            System.Drawing.Rectangle rect = new System.Drawing.Rectangle(0, 0, bitmap.Width, bitmap.Height);
            System.Drawing.Imaging.BitmapData data = bitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, bitmap.PixelFormat);

            // load the overlay pattern
            System.Drawing.Bitmap overlayBitmap = new System.Drawing.Bitmap("../overlays/Topographic.bmp");

            // prepare memory space for the newly created color data
            this.heightData = new byte[data.Stride * bitmap.Height];

            this.terrainWidth = bitmap.Width;
            this.terrainHeight = bitmap.Height;

            //viewport.MakeCurrent();

            // get a pointer to the to first line (=first pixel)
            IntPtr ptr = data.Scan0;

            // create a byte copy of the heightmap data
            System.Runtime.InteropServices.Marshal.Copy(ptr, this.heightData, 0, data.Stride * bitmap.Height);

            // release the lock, so this bitmap can be used elsewhere as well
            bitmap.UnlockBits(data);
            bitmap = null;


            

            // release some memory (to prevent OutOfMemory exception)
            original = null;

            // the vertex array for the model
            Vertex[] vertices = new Vertex[this.terrainWidth * this.terrainHeight * 6];

            // dimension multipliers
            float fWidth = 100f / (float) this.terrainWidth;
            float fHeight = 100f / (float) this.terrainHeight;

            // adjust the multipliers for non-square bitmaps
            if (originalHeight > originalWidth)
            {
                fWidth *= (float)originalWidth / (float)originalHeight;
            }
            else if (originalHeight < originalWidth)
            {
                fHeight*= (float)originalHeight / (float)originalWidth;
            }

            // build the model
            if(this.heightData != null){
                for (int y = 0; y < this.terrainHeight - 1; y++)
                {
                    float fy = (float)y;

                    // precalculate some stuff that stays constant for whole row
                    float yPos = fy * fHeight;
                    float yPosNext = (fy + 1) * fHeight;

                    for (int x = 0; x < this.terrainWidth - 1; x++)
                    {
                        float fx = (float)x;

                        // upper left point of current quad
                        Vertex a = new Vertex();
                        a.Position.X = fx * fWidth;
                        a.Position.Y = yPos;
                        a.Position.Z = (float)(this.heightData[(x + this.terrainWidth * y) * 4] * 0.005);
                        //a.Color = colors[this.heightData[(x + this.terrainWidth * y) * 4]];
                        a.TexCoord.X = fx * fWidth / 100f;
                        a.TexCoord.Y = yPos / 100f;

                        // upper right verex of current quad
                        Vertex b = new Vertex();
                        b.Position.X = (fx + 1) * fWidth;
                        b.Position.Y = yPos;
                        b.Position.Z = (float)(this.heightData[(x + 1 + this.terrainWidth * y) * 4] * 0.005);
                        //b.Color = colors[this.heightData[(x + 1 + this.terrainWidth * y) * 4]];
                        b.TexCoord.X = (fx + 1) * fWidth / 100f;
                        b.TexCoord.Y = yPos / 100f;

                        // bottom left verex of current quad
                        Vertex c = new Vertex();
                        c.Position.X = fx * fWidth;
                        c.Position.Y = yPosNext;
                        c.Position.Z = (float)(this.heightData[(x + this.terrainWidth * (y + 1)) * 4] * 0.005);
                        //c.Color = colors[this.heightData[(x  + this.terrainWidth * (y + 1)) * 4]];
                        c.TexCoord.X = fx * fWidth / 100f;
                        c.TexCoord.Y = yPosNext / 100f;

                        // bottom right verex of current quad
                        Vertex d = new Vertex();
                        d.Position.X = (fx + 1) * fWidth;
                        d.Position.Y = yPosNext;
                        d.Position.Z = (float)(this.heightData[(x + 1 + this.terrainWidth * (y + 1)) * 4] * 0.005);
                        //d.Color = colors[this.heightData[(x + 1 + this.terrainWidth * (y + 1)) * 4]];
                        d.TexCoord.X = (fx + 1) * fWidth / 100f;
                        d.TexCoord.Y = yPos / 100f;

                        // first triangle
                        vertices[(x + this.terrainWidth * y) * 6] = a;
                        vertices[(x + this.terrainWidth * y) * 6 + 1] = b;
                        vertices[(x + this.terrainWidth * y) * 6 + 2] = c;
                        
                        // second triangle
                        vertices[(x + this.terrainWidth * y) * 6 + 3] = b;
                        vertices[(x + this.terrainWidth * y) * 6 + 4] = d;
                        vertices[(x + this.terrainWidth * y) * 6 + 5] = c;
                    }
                }
            }

            try
            {
                main.Invoke(new System.Windows.Forms.MethodInvoker(delegate()
                {
                    // delete the previous buffer content
                    if (this.vertexBufferHandle != 0)
                    {
                        GL.DeleteBuffers(1, ref this.vertexBufferHandle);
                    }

                    // allocate the buffer
                    GL.GenBuffers(1, out this.vertexBufferHandle);

                    // tell that we are using that buffer
                    GL.BindBuffer(BufferTarget.ArrayBuffer, this.vertexBufferHandle);

                    // upload the data into the buffer into GPU
                    GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(vertices.Length * 5 * sizeof(float)), vertices, BufferUsageHint.StaticDraw);

                   

                    this.ApplyTexture();

                    main.Output3dButtonsOn();
                    this.viewport.Invalidate();

                    main.HideBuildingModel();
                }));
            }
            // this might throw exceptions in case the main thread was terminated while this thread is running
            catch (Exception) { };
        }

        public void Render()
        {
            GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);

            float sina = (float) Math.Sin(azimuth);
            float cosa = (float)Math.Cos(azimuth);
            float cose = (float)Math.Cos(elevation);

            Matrix4 modelview = Matrix4.LookAt(this.distance * cose * sina + this.targetX, this.distance * cose * cosa + this.targetY, this.distance * (float)Math.Sin(this.elevation), this.targetX, this.targetY, 0, 0, 0, 1);
            GL.MatrixMode(MatrixMode.Modelview);
            GL.LoadMatrix(ref modelview);

            //GL.Light(LightName.Light0, LightParameter.Position, new Vector4(1, 1, 0, 0));
            GL.Light(LightName.Light0, LightParameter.Ambient, new Vector4(0, 0, 0, 1));
            GL.Light(LightName.Light0, LightParameter.Diffuse, new Vector4(1, 1, 1 , 1));
            GL.Light(LightName.Light0, LightParameter.Specular, new Vector4(1, 1, 1, 1));
            //GL.Light(LightName.Light0, LightParameter.Ambient, new Vector4(0.2f, 0.2f, 0.2f, 1.0f));
            GL.Light(LightName.Light0, LightParameter.Diffuse, new Vector4(0.6f, 0.6f, 0.6f, 1.0f));
            //GL.Light(LightName.Light0, LightParameter.Position, new Vector4(0f, 1f, 1f, 0f));
            //GL.Light(LightName.Light0, LightParameter.SpotDirection, new Vector4(0f, 0f, 0f, 0f));
            //GL

            //GL.LightModel(LightModelParameter.LightModelAmbient, Vector4(0));
            //GL.Light(LightName.Light0, LightParameter.Diffuse, new Vector4(1, 1, 1, 1));

            //GL.Begin(BeginMode.Points);

            if (this.heightData != null)
            {
                // tell the OpenGL which buffer are we using
                GL.BindBuffer(BufferTarget.ArrayBuffer, this.vertexBufferHandle);
                GL.BindTexture(TextureTarget.ProxyTexture2D, this.textureHandle);
                
                // tell the format of the buffered data
                GL.InterleavedArrays(InterleavedArrayFormat.T2fV3f, 0, IntPtr.Zero);

                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)TextureMinFilter.Linear);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)TextureMagFilter.Linear);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapS, (int) TextureWrapMode.Clamp);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapR, (int)TextureWrapMode.Clamp);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapT, (int)TextureWrapMode.Clamp);

                // how to vertically scale the data
                GL.Scale(1f, 1f, this.heightScale);

                // read the data from buffer
                GL.DrawArrays(BeginMode.Triangles, 0, this.terrainWidth * this.terrainHeight * 6);
            }

            // display the stuff
            viewport.SwapBuffers();
        }

        public void ApplyTexture()
        {
            Main main = Main.Get();
            Config config = main.GetConfig();

            if (this.textureBase == null) return;

            if (this.textureHandle != 0) GL.DeleteTexture(this.textureHandle);

            string selected = (string)main.texture.Items[main.texture.SelectedIndex];

            string path = "";

            System.Drawing.Bitmap bitmap = null;

            // "Overlay: " type texture
            if(selected[0] == 'O'){
                path = config.overlayDirectory + "/" + selected.Substring(9, selected.Length - 9);
                bitmap = main.GetOutputManager().ApplyOverlay(new System.Drawing.Bitmap(this.textureBase), new System.Drawing.Bitmap(path));
            
            }

            // "Map: " type texture
            else if (selected[0] == 'M')
            {
                path = config.geoGenWorkingDirectory + "/" + selected.Substring(5, selected.Length - 5);
                System.Drawing.Bitmap original = new System.Drawing.Bitmap(path);

                bitmap = new System.Drawing.Bitmap(original, this.terrainWidth, this.terrainWidth);

                original = null;
            }

            System.Drawing.Rectangle rect = new System.Drawing.Rectangle(0, 0, bitmap.Width, bitmap.Height);
            System.Drawing.Imaging.BitmapData data = bitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, bitmap.PixelFormat);


            this.textureHandle = GL.GenTexture();
            GL.BindTexture(TextureTarget.Texture2D, this.textureHandle);

            GL.TexImage2D(TextureTarget.Texture2D, 0, PixelInternalFormat.Rgba, data.Width, data.Height, 0,
            OpenTK.Graphics.OpenGL.PixelFormat.Bgra, PixelType.UnsignedByte, data.Scan0);

            bitmap.UnlockBits(data);

            this.viewport.Invalidate();
        }

        public void SaveScreenshot()
        {
            Main main = Main.Get();

            if (main.saveOutputDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                this.viewport.GrabScreenshot().Save(main.saveOutputDialog.FileName, System.Drawing.Imaging.ImageFormat.Png);
            }
        }
    }
}
