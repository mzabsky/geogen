using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Media.Media3D;

namespace GeoGen_Studio
{
    /// <summary>
    /// Interaction logic for Output3D.xaml
    /// </summary>
    public partial class Output3D : UserControl
    {
        public Output3D()
        {
            InitializeComponent();
        }

        private void ClearViewport()
        {
            ModelVisual3D m;
            for (int i = mainViewport.Children.Count - 1; i >= 0; i--)
            {
                m = (ModelVisual3D)mainViewport.Children[i];
                if (m.Content is DirectionalLight == false)
                {
                    mainViewport.Children.Remove(m);
                }
            }
        }

        private Point3D[] GetRandomTopographyPoints()
        {
            //create a 10x10 topography.
            Point3D[] points = new Point3D[100];
            Random r = new Random();
            double y;
            double denom = 1000;
            int count = 0;
            for (int z = 0; z < 10; z++)
            {
                for (int x = 0; x < 10; x++)
                {
                    System.Threading.Thread.Sleep(1);
                    y = Convert.ToDouble(r.Next(1, 999)) / denom;
                    points[count] = new Point3D(x, y, z);
                    count += 1;
                }
            }
            return points;
        }

        private Vector3D CalculateNormal(Point3D p0, Point3D p1, Point3D p2)
        {
            Vector3D v0 = new Vector3D(
                p1.X - p0.X, p1.Y - p0.Y, p1.Z - p0.Z);
            Vector3D v1 = new Vector3D(
                p2.X - p1.X, p2.Y - p1.Y, p2.Z - p1.Z);
            return Vector3D.CrossProduct(v0, v1);
        }

        private Model3DGroup CreateTriangleModel(Point3D p0, Point3D p1, Point3D p2)
        {
            MeshGeometry3D mesh = new MeshGeometry3D();
            mesh.Positions.Add(p0);
            mesh.Positions.Add(p1);
            mesh.Positions.Add(p2);
            mesh.TriangleIndices.Add(0);
            mesh.TriangleIndices.Add(1);
            mesh.TriangleIndices.Add(2);
            Vector3D normal = CalculateNormal(p0, p1, p2);
            mesh.Normals.Add(normal);
            mesh.Normals.Add(normal);
            mesh.Normals.Add(normal);
            Material material = new DiffuseMaterial(
                new SolidColorBrush(Colors.DarkKhaki));
            GeometryModel3D model = new GeometryModel3D(
                mesh, material);
            Model3DGroup group = new Model3DGroup();
            group.Children.Add(model);
            return group;
        }

        public void SetTerrain(string path)
        {
            System.Drawing.Bitmap bitmap = new System.Drawing.Bitmap(path);
            
            ClearViewport();

            ImageBrush brush = new ImageBrush(new BitmapImage(
                    new Uri(path, UriKind.Relative)
                ));

            

            brush.TileMode = TileMode.Tile;

            DiffuseMaterial material = new DiffuseMaterial();
            material.Brush = new SolidColorBrush(Color.FromRgb(255, 0, 0));


            MeshGeometry3D mesh = new MeshGeometry3D();

            for(int y = 0; y < bitmap.Height; y++){
                for(int x = 0; x < bitmap.Width; x++){
                    mesh.Positions.Add(new Point3D(x - bitmap.Width / 2, y - bitmap.Height / 2, (Double)bitmap.GetPixel(x, y).R / 50.0));

                    mesh.TextureCoordinates.Add(new Point(x, y));
                }
            }

            for (int y = 0; y < bitmap.Height - 1; y++)
            {
                for (int x = 0; x < bitmap.Width - 1; x++)
                {
                    mesh.Normals.Add(this.CalculateNormal(
                        mesh.Positions[x + y * bitmap.Width],
                        
                        mesh.Positions[(x + 1) + (y + 1) * bitmap.Width],
                        mesh.Positions[x + (y + 1) * bitmap.Width]
                    ));

                    mesh.Normals.Add(this.CalculateNormal(
                        mesh.Positions[x + y * bitmap.Width],
                        
                        mesh.Positions[(x + 1) + y * bitmap.Width],
                        mesh.Positions[(x + 1) + (y + 1) * bitmap.Width]
                    ));

                    mesh.TriangleIndices.Add(x + y * bitmap.Width);
                    mesh.TriangleIndices.Add((x + 1) + (y + 1) * bitmap.Width);
                    mesh.TriangleIndices.Add(x + (y + 1) * bitmap.Width);

                    mesh.TriangleIndices.Add(x + y * bitmap.Width);
                    mesh.TriangleIndices.Add((x + 1) + y  * bitmap.Width);
                    mesh.TriangleIndices.Add((x + 1) + (y + 1) * bitmap.Width);
                    
                }
            }

           // GeometryModel3D model = ;

            /*Point3D[] points = GetRandomTopographyPoints();

            for (int z = 0; z <= 80; z = z + 10)
            {
                for (int x = 0; x < 9; x++)
                {
                    topography.Children.Add(
                        CreateTriangleModel(
                                points[x + z],
                                points[x + z + 10],
                                points[x + z + 1])
                    );
                    topography.Children.Add(
                        CreateTriangleModel(
                                points[x + z + 1],
                                points[x + z + 10],
                                points[x + z + 11])
                    );
                }
            }*/

            ModelVisual3D model = new ModelVisual3D();
            model.Content = new GeometryModel3D(mesh, material);

            ModelVisual3D light = new ModelVisual3D();
            light.Content = //new AmbientLight(Color.FromRgb(230, 215, 215));
                new DirectionalLight(Color.FromRgb(230, 215, 215), new Vector3D(-100, 100, -100));

            this.mainViewport.Children.Add(model);
            this.mainViewport.Children.Add(light);
        }
    }
}
