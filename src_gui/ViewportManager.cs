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
            public Vector3 Color;
            public Vector3 Position;
        }

        public OpenTK.GLControl viewport;
        public byte[] heightData;
        private int terrainHeight;
        private int terrainWidth;
        private bool loaded;

        public double azimuth = 0.785398; // 45 degrees
        public double elevation = 0.52; // 30 degrees
        public float distance = 100;

        private int vertexBufferHandle;

        private float heightScale = 8f;

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

        Vertex[] v;

        public void Init()
        {
            Matrix4 projection = Matrix4.CreatePerspectiveFieldOfView((float)Math.PI / 4, this.viewport.Width / (float)this.viewport.Height, 1.0f, 3000.0f);
            GL.MatrixMode(MatrixMode.Projection);
            GL.LoadMatrix(ref projection);
            GL.Enable(EnableCap.DepthTest);
            GL.Viewport(0, 0, this.viewport.Width, this.viewport.Height); // Use all of the glControl painting are

            //GL.Enable(EnableCap.Lighting);
            GL.Enable(EnableCap.Light0);
            GL.Enable(EnableCap.ColorMaterial);
            GL.ColorMaterial(MaterialFace.FrontAndBack, ColorMaterialParameter.AmbientAndDiffuse);
            //GL.ShadeModel(ShadingModel.Smooth);
            
            GL.EnableClientState(EnableCap.ColorArray);

            GL.EnableClientState(EnableCap.VertexArray);

            
  
        }

        private Vector3 ColorToVector(System.Drawing.Color color)
        {
            return new Vector3((float)color.R / 255f, (float)color.G / 255f, (float)color.B / 255f);
        }

        public void SetupViewport(){

        }

        public void SetWireframeState(bool wireframe){
            if (!wireframe)
            {
                GL.PolygonMode(MaterialFace.FrontAndBack, PolygonMode.Fill);
            }
            else
            {
                GL.PolygonMode(MaterialFace.FrontAndBack, PolygonMode.Line);
            }

            this.viewport.Invalidate();
        }

        public void SetTerrain(string path){
            Main main = Main.Get();
            Config config = main.GetConfig();

            System.Drawing.Bitmap original = new System.Drawing.Bitmap(path);

            System.Drawing.Bitmap bitmap = new System.Drawing.Bitmap(original, Math.Min(original.Width, (int)config.ModelDetailLevel), Math.Min(original.Height, (int)config.ModelDetailLevel));

            // prepare byte access to the height data
            System.Drawing.Rectangle rect = new System.Drawing.Rectangle(0, 0, bitmap.Width, bitmap.Height);
            System.Drawing.Imaging.BitmapData data = bitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, bitmap.PixelFormat);

            byte[] copy = new byte[data.Stride * bitmap.Height];

            System.Drawing.Bitmap overlayBitmap = new System.Drawing.Bitmap("../overlays/Topographic.bmp");

            Vector3[] colors = new Vector3[256];

            for (int x = 0; x < 256; x++)
            {
                colors[x] = this.ColorToVector(overlayBitmap.GetPixel(x, 0));
            }

                // prepare memory space for the newly created color data
                //this.heightData = new byte[bitmap.Width * bitmap.Height];

            this.heightData = new byte[data.Stride * bitmap.Height];

            this.terrainWidth = bitmap.Width;
            this.terrainHeight = bitmap.Height;

            viewport.MakeCurrent();

            // get a pointer to the to first line (=first pixel)
            IntPtr ptr = data.Scan0;

            // create a byte copy of the heightmap data
            System.Runtime.InteropServices.Marshal.Copy(ptr, this.heightData, 0, data.Stride * bitmap.Height);

            bitmap.UnlockBits(data);

            /*for (int y = 0; y < this.terrainHeight; y++)
            {
                for (int x = 0; x < this.terrainWidth; x++)
                {
                    this.heightData[x + this.terrainWidth * y] = (byte) ((int)(bitmap.GetPixel(x, y).R));
                }
            }*/

            Vertex[] vertices = new Vertex[this.terrainWidth * this.terrainHeight * 6];

            
            float fWidth = 100f / (float) this.terrainWidth;
            float fHeight = 100f / (float) this.terrainHeight;

            if (original.Height > original.Width)
            {
                fWidth *= (float)original.Width / (float)original.Height;
            }
            else if (original.Height < original.Width)
            {
                fHeight*= (float)original.Height / (float)original.Width;
            }

            if(this.heightData != null){
                for (int y = 0; y < this.terrainHeight - 1; y++)
                {
                    float fy = (float)y;

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
                        a.Color = colors[this.heightData[(x + this.terrainWidth * y) * 4]];

                        // upper right verex of current quad
                        Vertex b = new Vertex();
                        b.Position.X = (fx + 1) * fWidth;
                        b.Position.Y = yPos;
                        b.Position.Z = (float)(this.heightData[(x + 1 + this.terrainWidth * y) * 4] * 0.005);
                        b.Color = colors[this.heightData[(x + 1 + this.terrainWidth * y) * 4]];

                        // bottom left verex of current quad
                        Vertex c = new Vertex();
                        c.Position.X = fx * fWidth;
                        c.Position.Y = yPosNext;
                        c.Position.Z = (float)(this.heightData[(x + this.terrainWidth * (y + 1)) * 4] * 0.005);
                        c.Color = colors[this.heightData[(x  + this.terrainWidth * (y + 1)) * 4]];

                        // bottom right verex of current quad
                        Vertex d = new Vertex();
                        d.Position.X = (fx + 1) * fWidth;
                        d.Position.Y = yPosNext;
                        d.Position.Z = (float)(this.heightData[(x + 1 + this.terrainWidth * (y + 1)) * 4] * 0.005);
                        d.Color = colors[this.heightData[(x + 1 + this.terrainWidth * (y + 1)) * 4]];

                        // first triangle
                        vertices[(x + this.terrainWidth * y) * 6] = a;
                        vertices[(x + this.terrainWidth * y) * 6 + 1] = b;
                        vertices[(x + this.terrainWidth * y) * 6 + 2] = c;

                        //GL.Color3(1.0f, 1.0f, 1.0f); GL.Vertex3(fx * 100 / fWidth,          fy * 100 / fHeight,         this.heightData[x + this.terrainWidth * y] * 0.05);
                        //GL.Color3(1.0f, 0.0f, 0.0f); GL.Vertex3((fx + 1) * 100 / fWidth, fy * 100 / fHeight, this.heightData[x + 1 + this.terrainWidth * y] * 0.05);
                        //GL.Color3(0.2f, 0.9f, 1.0f); GL.Vertex3(fx * 100 / fWidth, (fy + 1) * 100 / fHeight, this.heightData[x + this.terrainWidth * (y + 1)] * 0.05);


                        // second triangle
                        vertices[(x + this.terrainWidth * y) * 6 + 3] = b;
                        vertices[(x + this.terrainWidth * y) * 6 + 4] = d;
                        vertices[(x + this.terrainWidth * y) * 6 + 5] = c;

                        /*
                        GL.Color3(1.0f, 1.0f, 1.0f); GL.Vertex3((fx + 1) * 100 / fWidth, fy * 100 / fHeight, this.heightData[x + 1 + this.terrainWidth * y] * 0.05);
                        GL.Color3(1.0f, 0.0f, 0.0f); GL.Vertex3((fx + 1) * 100 / fWidth, (fy + 1) * 100 / fHeight, this.heightData[x + 1 + this.terrainWidth * (y + 1)] * 0.05);
                        GL.Color3(0.2f, 0.9f, 1.0f); GL.Vertex3(fx * 100 / fWidth, (fy + 1) * 100 / fHeight, this.heightData[x + this.terrainWidth * (y + 1)] * 0.05);*/
                    }
                }
            }

            if (this.vertexBufferHandle != 0)
            {
                GL.DeleteBuffers(1, ref this.vertexBufferHandle);
            }

            GL.GenBuffers(1, out this.vertexBufferHandle);

            GL.BindBuffer(BufferTarget.ArrayBuffer, this.vertexBufferHandle);

            GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(vertices.Length * 6 * sizeof(float)), vertices, BufferUsageHint.StaticDraw);

            this.v = vertices;

            this.viewport.Invalidate();
        }

        public void Render()
        {
            GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);

            float sina = (float) Math.Sin(azimuth);
            float cosa = (float)Math.Cos(azimuth);
            float cose = (float)Math.Cos(elevation);

            Matrix4 modelview = Matrix4.LookAt(this.distance * cose * sina + 50, this.distance * cose * cosa + 50, this.distance * (float)Math.Sin(this.elevation), 50, 50, 0, 0, 0, 1);
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

            ErrorCode err;

            if (this.heightData != null)
            {
                //GL.BindBuffer(BufferTarget.ArrayBuffer, this.vertexBufferHandle);

                //err = GL.GetError();
                //GL.DrawElements(BeginMode.Triangles, this.terrainWidth * this.terrainHeight * 6, DrawElementsType.UnsignedInt, 0);

                //GL.InterleavedArrays(InterleavedArrayFormat.C3fV3f, 0, IntPtr.Zero);

                GL.BindBuffer(BufferTarget.ArrayBuffer, this.vertexBufferHandle);

                //GL.VertexPointer(3, VertexPointerType.Float, 6 * sizeof(float), IntPtr.Zero);
                //GL.ColorPointer(3, ColorPointerType.Float, 6 * sizeof(float), 3);

                GL.InterleavedArrays(InterleavedArrayFormat.C3fV3f, 0, IntPtr.Zero);

                GL.Scale(1f, 1f, this.heightScale);

                GL.DrawArrays(BeginMode.Triangles, 0, v.Length);

                
                    //err = GL.GetError();

                /*for (uint i = 0; i < v.Length; i++)
                {
                    GL.Color3(v[i].Color);
                    GL.Vertex3(v[i].Position);
                }*/

            }

            /*GL.Color3(1.0f, 1.0f, 0.0f); GL.Vertex3(-1.0f, -1.0f, 4.0f);
            GL.Color3(1.0f, 0.0f, 0.0f); GL.Vertex3(1.0f, -1.0f, 4.0f);
            GL.Color3(0.2f, 0.9f, 1.0f); GL.Vertex3(0.0f, 1.0f, 4.0f);

            GL.Color3(1.0f, 1.0f, 1.0f); GL.Vertex3(2.0f, 1.0f, 4.0f);
            GL.Color3(1.0f, 0.0f, 0.0f); GL.Vertex3(1.0f, -1.0f, 4.0f);
            GL.Color3(0.2f, 0.9f, 1.0f); GL.Vertex3(0.0f, 1.0f, 4.0f);*/

           
           // GL.End();

            if (this.heightData != null)
            {
                err = GL.GetError();
            }

            viewport.SwapBuffers();
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
