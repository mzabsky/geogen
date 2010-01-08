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

        public OpenTK.GLControl viewport;
        public OutputManager.SHData heightData;

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
            this.SetupViewport();
        }

        public void SetupViewport(){
            Matrix4 projection = Matrix4.CreatePerspectiveFieldOfView((float)Math.PI / 4, this.viewport.Width / (float)this.viewport.Height, 0.1f, 3000.0f);
            GL.MatrixMode(MatrixMode.Projection);
            GL.LoadMatrix(ref projection);
            GL.Enable(EnableCap.DepthTest);
            GL.Enable(EnableCap.Texture2D);
            GL.Enable(EnableCap.Multisample);
            GL.Enable(EnableCap.RescaleNormal);
            GL.Enable(EnableCap.Normalize);
            GL.Viewport(0, 0, this.viewport.Width, this.viewport.Height); // Use all of the glControl painting are

            GL.Enable(EnableCap.Lighting);
            GL.Enable(EnableCap.Light0);
            GL.Enable(EnableCap.ColorMaterial);
            GL.ColorMaterial(MaterialFace.Front, ColorMaterialParameter.AmbientAndDiffuse);
            GL.ShadeModel(ShadingModel.Smooth);


            //GL.EnableClientState(EnableCap.ColorArray);

            GL.EnableClientState(EnableCap.VertexArray);
            GL.EnableClientState(EnableCap.TextureCoordArray);
            GL.EnableClientState(EnableCap.NormalArray);
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

            main.outputs3d.Items.Clear();

            // remove "Maps:" entries from the texture list
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

        Vector3 CalculateNormal(Vector3 a, Vector3 b, Vector3 c)
        {
            // calculate the vectors A and B
            // note that v[3] is defined with counterclockwise winding in mind
            // a
            float dX1 = a.X - b.X;
            float dY1 = a.Y - b.Y;
            float dZ1 = a.Z - b.Z;
            // b
            float dX2 = b.X - c.X;
            float dY2 = b.Y - c.Y;
            float dZ2 = b.Z - c.Z;
         
            Vector3 normal = new Vector3();

            // calculate the cross product and place the resulting vector
            // into the address specified by vertex_t *normal
            //normal.X = (a.y * b.z) - (a.z * b.y);
            //normal.Y = (a.z * b.x) - (a.x * b.z);
            //normal.Z = (a.x * b.y) - (a.y * b.x);

            normal.X = (dY1 * dZ2) - (dZ1 * dY2);
            normal.Y = (dZ1 * dX2) - (dX1 * dZ2);
            normal.Z = (dX1 * dY2) - (dY1 * dX2);

            normal.Normalize();

            return  normal;
        }


        public void SetTerrain(string path){
            Main main = Main.Get();
            Config config = main.GetConfig();

            // the original map (as generated)
            //System.Drawing.Bitmap original = new System.Drawing.Bitmap(path);
            OutputManager.SHData original = new OutputManager.SHData(path);

            // store original's size
            int originalHeight = original.height;
            int originalWidth = original.width;

            // resized bitmap (from which the model will be generated 1:1)
            //System.Drawing.Bitmap bitmap = new System.Drawing.Bitmap(original, , );

            

            // prepare byte access to the height data
            //System.Drawing.Rectangle rect = new System.Drawing.Rectangle(0, 0, bitmap.Width, bitmap.Height);
            //System.Drawing.Imaging.BitmapData data = bitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, bitmap.PixelFormat);

            

            // load the overlay pattern
            System.Drawing.Bitmap overlayBitmap = new System.Drawing.Bitmap("../overlays/Topographic.bmp");

            // prepare memory space for the newly created color data
            this.heightData = original.GetResized(Math.Min(original.width, (int)config.ModelDetailLevel), Math.Min(original.height, (int)config.ModelDetailLevel));

            this.textureBase = this.heightData.GetBitmap();

            //viewport.MakeCurrent();

            // get a pointer to the to first line (=first pixel)
            //IntPtr ptr = data.Scan0;

            // create a byte copy of the heightmap data
            //System.Runtime.InteropServices.Marshal.Copy(ptr, this.heightData, 0, data.Stride * bitmap.Height);

            // release the lock, so this bitmap can be used elsewhere as well
            //bitmap.UnlockBits(data);
            //bitmap = null;


            

            // release some memory (to prevent OutOfMemory exception)
            original = null;

            // the vertex array for the model
            Vertex[] vertices = new Vertex[this.heightData.width * this.heightData.height * 6];

            // dimension multipliers
            float fWidth = 100f / (float) this.heightData.width;
            float fHeight = 100f / (float) this.heightData.height;

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
                for (int y = 0; y < this.heightData.height - 1; y++)
                {
                    float fy = (float)y;

                    // precalculate some stuff that stays constant for whole row
                    float yPos = (fy + 0.5f) * fHeight;
                    float yPosNext = (fy + 1 + 0.5f) * fHeight;

                    for (int x = 0; x < this.heightData.width - 1; x++)
                    {
                        float fx = (float)x;

                        // upper left point of current quad
                        Vertex a = new Vertex();
                        a.Position.X = fx * fWidth;
                        a.Position.Y = yPos;
                        a.Position.Z = (float)((float)this.heightData.data[(x + this.heightData.width * y)] * 0.005f / 128f);
                        //a.Color = colors[this.heightData[(x + this.terrainWidth * y) * 4]];
                        a.TexCoord.X = (fx  + 0.5f) * fWidth / 100f;
                        a.TexCoord.Y = yPos / 100f;

                        // upper right verex of current quad
                        Vertex b = new Vertex();
                        b.Position.X = (fx + 1) * fWidth;
                        b.Position.Y = yPos;
                        b.Position.Z = (float)((float)this.heightData.data[(x + 1 + this.heightData.width * y)] * 0.005f / 128f);
                        //b.Color = colors[this.heightData[(x + 1 + this.terrainWidth * y) * 4]];
                        b.TexCoord.X = (fx + 1 + 0.5f) * fWidth / 100f;
                        b.TexCoord.Y = yPos / 100f;

                        // bottom left verex of current quad
                        Vertex c = new Vertex();
                        c.Position.X = fx * fWidth;
                        c.Position.Y = yPosNext;
                        c.Position.Z = (float)((float)this.heightData.data[(x + this.heightData.width * (y + 1))] * 0.005f / 128f);
                        //c.Color = colors[this.heightData[(x  + this.terrainWidth * (y + 1)) * 4]];
                        c.TexCoord.X = (fx + 0.5f) * fWidth / 100f;
                        c.TexCoord.Y = yPosNext / 100f;

                        // bottom right verex of current quad
                        Vertex d = new Vertex();
                        d.Position.X = (fx + 1) * fWidth;
                        d.Position.Y = yPosNext;
                        d.Position.Z = (float)((float)this.heightData.data[(x + 1 + this.heightData.width * (y + 1))] * 0.005f / 128f);
                        //d.Color = colors[this.heightData[(x + 1 + this.terrainWidth * (y + 1)) * 4]];
                        d.TexCoord.X = (fx + 1 + 0.5f) * fWidth / 100f;
                        d.TexCoord.Y = yPosNext / 100f;

                        // crop underwater heights if requested
                        if (!config.enableTerrainUnderZero)
                        {
                            if (a.Position.Z < 0) a.Position.Z = 0;
                            if (b.Position.Z < 0) b.Position.Z = 0;
                            if (c.Position.Z < 0) c.Position.Z = 0;
                            if (d.Position.Z < 0) d.Position.Z = 0;
                        }

                        Vertex b2 = new Vertex(b);
                        Vertex c2 = new Vertex(c);

                        a.Normal = this.CalculateNormal(c.Position, a.Position, b.Position);
                        d.Normal = this.CalculateNormal(b.Position, d.Position, c.Position);

                        b.Normal = a.Normal;
                        c.Normal = a.Normal;

                        b2.Normal = d.Normal;
                        c2.Normal = d.Normal;

                        // first triangle
                        vertices[(x + this.heightData.width * y) * 6] = a;
                        vertices[(x + this.heightData.width * y) * 6 + 1] = b;
                        vertices[(x + this.heightData.width * y) * 6 + 2] = c;
                        
                        // second triangle                        
                        vertices[(x + this.heightData.width * y) * 6 + 4] = d;
                        vertices[(x + this.heightData.width * y) * 6 + 5] = c2; 
                        vertices[(x + this.heightData.width * y) * 6 + 3] = b2; 
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
                    GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(vertices.Length * 8 * sizeof(float)), vertices, BufferUsageHint.StaticDraw);

                   
                    // rebuild the texture
                    this.ApplyTexture();

                    // UI stuff
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

            //GL.Material(MaterialFace.Front, MaterialParameter.Diffuse, new Vector4(1, 1, 1, 1));
            //GL.Material(MaterialFace.Front, MaterialParameter.Shininess, new Vector4(2f, 2f, 2f, 1f));

            //GL.Light(LightName.Light0, LightParameter.Position, new Vector4(1, 1, 0, 0));
            GL.Light(LightName.Light0, LightParameter.Ambient, new Vector4(0.2f, 0.2f, 0.2f, 1));
            GL.Light(LightName.Light0, LightParameter.Diffuse, new Vector4(0.6f, 0.6f, 0.6f, 1));
            //GL.Light(LightName.Light0, LightParameter.SpotExponent, new Vector4(2, 2, 2, 2));
            GL.Light(LightName.Light0, LightParameter.Specular, new Vector4(0, 0, 0, 0));
            //GL.Light(LightName.Light0, LightParameter.Ambient, new Vector4(0.2f, 0.2f, 0.2f, 1.0f));
            //GL.Light(LightName.Light0, LightParameter.Diffuse, new Vector4(1f, 1f, 1f, 1.0f));
            //GL.Light(LightName.Light0, LightParameter.Position, new Vector4(0f, 50f, 50f, 0f));
            //GL.Light(LightName.Light0, LightParameter.SpotDirection, new Vector4(-1, 1, -1, 0f));
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
                GL.InterleavedArrays(InterleavedArrayFormat.T2fN3fV3f, 0, IntPtr.Zero);

                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)TextureMinFilter.Linear);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)TextureMagFilter.Linear);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapS, (int) TextureWrapMode.Clamp);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapR, (int)TextureWrapMode.Clamp);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapT, (int)TextureWrapMode.Clamp);

                // how to vertically scale the data
                GL.Scale(1f, 1f, this.heightScale);

                // read the data from buffer
                GL.DrawArrays(BeginMode.Triangles, 0, this.heightData.width * this.heightData.height * 6);
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
                bitmap = main.GetOutputManager().ApplyOverlay(this.heightData, new System.Drawing.Bitmap(path));
            
            }

            // "Map: " type texture
            else if (selected[0] == 'M')
            {
                path = config.geoGenWorkingDirectory + "/" + selected.Substring(5, selected.Length - 5);

                bitmap = new OutputManager.SHData(path).GetBitmap();

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
