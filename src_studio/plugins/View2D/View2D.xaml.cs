using System;
using System.ComponentModel;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using GeoGen.Studio.Utilities;
using GeoGen.Studio.Utilities.Configurability;

namespace GeoGen.Studio.PlugIns
{    
    public partial class View2D : INotifyPropertyChanged
    {
        private bool scrolling;
        private double lastDragX;
        private double lastDragY;
        private double dragPointX;
        private double dragPointY;

        private const double MinimumVisibleTip = 20;

        public IGenerator Generator { get; private set; }

        public HeightData SelectedMap { get; set; }

        [Configurable(UseEmptyInstanceAsDefault = true)]
        public Overlay SelectedOverlay { get; set; }

        public double ZoomFactor { get; set; }

        public double MouseX { get; private set; }

        public double MouseY { get; private set; }

        public int? SelectedValueX { get; private set; }

        public int? SelectedValueY { get; private set; }

        public int? SelectedValue { get; private set; }

        public bool IsMouseOverMap { get; private set; }

        public View2D()
        {
            this.ZoomFactor = 1;

            InitializeComponent();
            MainConfig.Register(this);

            var items = toolbar.Items;

            this.image.MouseDown += delegate(object sender, MouseButtonEventArgs args)
            {
                if (args.ChangedButton != MouseButton.Left)
                {
                    return;
                }

                this.dragPointX = Mouse.GetPosition(this.image).X;
                this.dragPointY = Mouse.GetPosition(this.image).Y;
                this.lastDragX = (double) this.image.GetValue(Canvas.LeftProperty) + dragPointX;  //Mouse.GetPosition(this.canvas).X;
                this.lastDragY = (double)this.image.GetValue(Canvas.TopProperty) + dragPointY;

                this.image.CaptureMouse();
            };

            this.image.LostMouseCapture += delegate(object sender, MouseEventArgs args){
                this.image.ReleaseMouseCapture();
            };

            this.image.MouseUp += delegate(object sender, MouseButtonEventArgs args)
            {
                this.image.ReleaseMouseCapture();
            };

            this.image.TextInput += delegate(object sender, TextCompositionEventArgs args)
            {
                this.image.ReleaseMouseCapture();
            };

            this.canvas.MouseWheel += delegate(object sender, MouseWheelEventArgs args) {
                if (this.IsMouseOverMap)
                {
                    if (args.Delta > 0) this.ZoomIn();
                    else this.ZoomOut();
                }
            };

            this.image.MouseMove += delegate(object sender, MouseEventArgs args)
            {
                if (this.image.IsMouseCaptured)
                {
                    //this.image.SetValue(Canvas.LeftProperty, Mouse.GetPosition(this.image).X - this.dragStartX);

                    this.image.SetValue(Canvas.LeftProperty, (double)image.GetValue(Canvas.LeftProperty) - (this.lastDragX - Mouse.GetPosition(this.canvas).X));
                    this.image.SetValue(Canvas.TopProperty, (double)image.GetValue(Canvas.TopProperty) - (this.lastDragY - Mouse.GetPosition(this.canvas).Y));

                    this.lastDragX = (double)this.image.GetValue(Canvas.LeftProperty) + dragPointX;  //Mouse.GetPosition(this.canvas).X;
                    this.lastDragY = (double)this.image.GetValue(Canvas.TopProperty) + dragPointY;

                    this.ClampImagePosition();
                }
                else
                {
                    this.MouseX = Mouse.GetPosition(this.image).X;
                    this.MouseY = Mouse.GetPosition(this.image).Y;

                    this.SelectedValueX = (int)Math.Round(this.MouseX / this.ZoomFactor);
                    this.SelectedValueY = (int)Math.Round(this.MouseY / this.ZoomFactor);
                    this.SelectedValue = this.SelectedMap[(int)this.SelectedValueX, (int)this.SelectedValueY];

                    this.IsMouseOverMap = true;
                }
            };

            this.canvas.SizeChanged += delegate
            {
                this.ReleaseMouseCapture();
                this.ClampImagePosition();
            };

            this.image.MouseLeave += delegate
            {
                this.SelectedValueX = null;
                this.SelectedValueY = null;
                this.SelectedValue = null;
                this.IsMouseOverMap = false;
            };
        }

        public void Register(IGenerator generator)
        {
            this.Generator = generator;

            generator.Finished += delegate
            {
                this.SelectedMap = generator.Maps[0];
            };
        }

        public void Register(IDockManager dockManager)
        {
            dockManager.AddAsDockableContent(this, "2D View", true);            
        }

        public void ZoomIn()
        {
            if (this.ZoomFactor > 10) return;
            
            this.ZoomFactor *= 1.25;

            this.image.Width = image.ActualWidth * 1.25;
            this.image.Height = image.ActualHeight * 1.25;

            // make sure the zooming is centered on mouse
            this.image.SetValue(Canvas.LeftProperty, (double)image.GetValue(Canvas.LeftProperty) - (image.Width - image.ActualWidth) / (image.ActualWidth / Mouse.GetPosition(image).X));
            this.image.SetValue(Canvas.TopProperty, (double)image.GetValue(Canvas.TopProperty) - (image.Height - image.ActualHeight) / (image.ActualHeight / Mouse.GetPosition(image).Y));

            this.ClampImagePosition();
        }

        public void ZoomOut()
        {            
            if (this.ZoomFactor < 0.1) return;

            this.ZoomFactor *= 0.8;

            this.image.Width = image.ActualWidth * 0.80;
            this.image.Height = image.ActualHeight * 0.80;

            // make sure the zooming is centered on mouse
            this.image.SetValue(Canvas.LeftProperty, (double)image.GetValue(Canvas.LeftProperty) - (image.Width - image.ActualWidth) / (image.ActualWidth / Mouse.GetPosition(image).X));
            this.image.SetValue(Canvas.TopProperty, (double)image.GetValue(Canvas.TopProperty) - (image.Height - image.ActualHeight) / (image.ActualHeight / Mouse.GetPosition(image).Y));

            this.ClampImagePosition();
        }

        public void SaveImage()
        {
            GeoGen.Studio.UI.MessageBox.Show("Saved");
        }

        private void ClampImagePosition(){
            if ((double) this.image.GetValue(Canvas.LeftProperty) + this.image.ActualWidth < View2D.MinimumVisibleTip)
            {
                this.image.SetValue(Canvas.LeftProperty, View2D.MinimumVisibleTip - this.image.ActualWidth);
            }

            if (this.canvas.ActualWidth - (double)this.image.GetValue(Canvas.LeftProperty) < View2D.MinimumVisibleTip)
            {
                this.image.SetValue(Canvas.LeftProperty, this.canvas.ActualWidth - View2D.MinimumVisibleTip);
            }

            if ((double)this.image.GetValue(Canvas.TopProperty) + this.image.ActualHeight < View2D.MinimumVisibleTip)
            {
                this.image.SetValue(Canvas.TopProperty, View2D.MinimumVisibleTip - this.image.ActualHeight);
            }

            if (this.canvas.ActualHeight - (double)this.image.GetValue(Canvas.TopProperty) < View2D.MinimumVisibleTip)
            {
                this.image.SetValue(Canvas.TopProperty, this.canvas.ActualHeight - View2D.MinimumVisibleTip);
            }
        }
    }
}
