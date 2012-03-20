namespace GeoGen.Studio.PlugIns
{
    using System;
    using System.ComponentModel;
    using System.IO;
    using System.Linq;
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Data;
    using System.Windows.Documents;
    using System.Windows.Input;
    using System.Windows.Media.Imaging;

    using GeoGen.Studio.PlugInLoader;
    using GeoGen.Studio.PlugIns.Extensions;
    using GeoGen.Studio.PlugIns.Interfaces;
    using GeoGen.Studio.PlugIns.StatusBars;
    using GeoGen.Studio.Utilities;
    using GeoGen.Studio.Utilities.Context;
    using GeoGen.Studio.Utilities.IO;
    using GeoGen.Studio.Utilities.Messaging;
    using GeoGen.Studio.Utilities.Persistence;

    /// <summary>
    /// Displays generated maps in a 2D view sub-window.
    /// </summary>
    public partial class View2D : INotifyPropertyChanged
    {
        /// <summary>
        /// Minimum portion of image in display units which has to be on screen.
        /// </summary>
        private const double MinimumVisibleTip = 20;

        /// <summary>
        /// By how much is the image zoom factor multiplied when zooming in.
        /// </summary>
        private const double ZoomInStep = 1.25;

        /// <summary>
        /// By how much is the image zoom factor multiplied when zooming out.
        /// </summary>
        private const double ZoomOutStep = 0.8;

        /// <summary>
        /// Maxiumum zoom factor.
        /// </summary>
        private const double MaximumZoomFactor = 10;

        /// <summary>
        /// Minimum zoom factor.
        /// </summary>
        private const double MinimumZoomFactor = 0.1;

        /// <summary>
        /// Adorner layer used to gray out the control when there are no maps.
        /// </summary>
        private readonly Adorner adorner;

        /// <summary>
        /// <see cref="Context"/> used to display coordinates on the status bar.
        /// </summary>
        private readonly Context mouseOverContext = new Context("View 2D");

        /// <summary>
        /// Last known dragging X of the image relative to the upper-left corner of the canvas.
        /// </summary>
        private double lastDragX;

        /// <summary>
        /// Last known dragging y of the image relative to the upper-left corner of the canvas.
        /// </summary>
        private double lastDragY;

        /// <summary>
        /// X coordinate which is the user holding with his mouse while dragging the image.
        /// </summary>
        private double dragClickX;

        /// <summary>
        /// Y coordinate which is the user holding with his mouse while dragging the image.
        /// </summary>
        private double dragClickY;

        /// <summary>
        /// Backing field for <see cref="ZoomInCommand"/>.
        /// </summary>
        private ICommand zoomInCommand;

        /// <summary>
        /// Backing field for <see cref="ZoomOutCommand"/>.
        /// </summary>
        private ICommand zoomOutCommand;

        /// <summary>
        /// Backing field for <see cref="SaveImageCommand"/>.
        /// </summary>
        private ICommand saveImageCommand;

        /// <summary>
        /// Initializes a new instance of the <see cref="View2D"/> class.
        /// </summary>
        public View2D()
        {
            this.ZoomFactor = 1;

            InitializeComponent();
            MainConfig.Register(this);

            this.adorner = new GeoGen.Studio.UI.BusyAdorner(this.panel2D);
            var adornerLayer = AdornerLayer.GetAdornerLayer(this.panel2D);
            adornerLayer.Add(this.adorner);

            this.image.MouseDown += delegate(object sender, MouseButtonEventArgs args)
                {
                    if (args.ChangedButton != MouseButton.Left)
                    {
                        return;
                    }

                    this.dragClickX = Mouse.GetPosition(this.image).X;
                    this.dragClickY = Mouse.GetPosition(this.image).Y;
                    this.lastDragX = (double)this.image.GetValue(Canvas.LeftProperty) + this.dragClickX;
                    this.lastDragY = (double)this.image.GetValue(Canvas.TopProperty) + this.dragClickY;

                    this.image.CaptureMouse();
                };

            this.image.LostMouseCapture += delegate { this.image.ReleaseMouseCapture(); };

            this.image.MouseUp += delegate { this.image.ReleaseMouseCapture(); };

            this.image.TextInput += delegate { this.image.ReleaseMouseCapture(); };

            this.canvas.MouseWheel += delegate(object sender, MouseWheelEventArgs args)
                {
                    if (this.IsMouseOverMap)
                    {
                        if (args.Delta > 0)
                        {
                            this.ZoomInCommand.Execute(null);
                        }
                        else
                        {
                            this.ZoomOutCommand.Execute(null);
                        }
                    }
                };

            this.image.MouseMove += delegate
                {
                    if (this.image.IsMouseCaptured)
                    {
                        this.image.SetValue(
                            Canvas.LeftProperty,
                            (double)image.GetValue(Canvas.LeftProperty) - (this.lastDragX - Mouse.GetPosition(this.canvas).X));

                        this.image.SetValue(
                            Canvas.TopProperty,
                            (double)image.GetValue(Canvas.TopProperty) - (this.lastDragY - Mouse.GetPosition(this.canvas).Y));

                        this.lastDragX = (double)this.image.GetValue(Canvas.LeftProperty) + this.dragClickX;
                        this.lastDragY = (double)this.image.GetValue(Canvas.TopProperty) + this.dragClickY;

                        this.ClampImagePosition();
                    }
                    else
                    {
                        this.MouseX = Mouse.GetPosition(this.image).X;
                        this.MouseY = Mouse.GetPosition(this.image).Y;

                        // Update the status bar values
                        this.SelectedValueX = (int)Math.Round(this.MouseX / this.ZoomFactor);
                        this.SelectedValueY = (int)Math.Round(this.MouseY / this.ZoomFactor);

                        if (this.SelectedValueX > 0 && this.SelectedValueY > 0 && this.SelectedValueX < this.SelectedMap.Size.Width
                            && this.SelectedValueY < this.SelectedMap.Size.Height)
                        {
                            this.SelectedValue = this.SelectedMap[(int)this.SelectedValueX, (int)this.SelectedValueY];
                        }

                        this.IsMouseOverMap = true;
                    }
                };

            this.canvas.SizeChanged += delegate
                {
                    this.ReleaseMouseCapture();
                    this.ClampImagePosition();                    
                };

            this.image.MouseEnter += delegate { ContextManager.EnterContext(this.mouseOverContext); };

            this.image.MouseLeave += delegate
                {
                    this.SelectedValueX = null;
                    this.SelectedValueY = null;
                    this.SelectedValue = null;
                    this.IsMouseOverMap = false;

                    ContextManager.LeaveContext(this.mouseOverContext);
                };

            this.image.SizeChanged += delegate
                {
                    this.ClampImagePosition();
                };

            this.PropertyChanged += delegate(object o, PropertyChangedEventArgs args)
            {
                if (args.PropertyName == "IsOverlayEnabled")
                {
                    Console.Write("a");
                }
            };
        }

        /// <summary>
        /// Gets the dock manager.
        /// </summary>
        public IDockManager DockManager { get; private set; }

        /// <summary>
        /// Gets the map generator.
        /// </summary>
        public IGenerator Generator { get; private set; }

        /// <summary>
        /// Gets or sets the selected map.
        /// </summary>
        /// <value>
        /// The selected map.
        /// </value>
        public HeightData SelectedMap { get; set; }

        /// <summary>
        /// Gets or sets the selected overlay.
        /// </summary>
        /// <value>
        /// The selected overlay.
        /// </value>
        [Persistent(UseEmptyInstanceAsDefault = true)]
        public Overlay SelectedOverlay { get; set; }

        /// <summary>
        /// Gets or sets the zoom factor (number expressing percentage of display size of the image).
        /// </summary>
        /// <value>
        /// The zoom factor.
        /// </value>
        public double ZoomFactor { get; set; }

        /// <summary>
        /// Gets last known X coordinate of mouse relative to upper left corner of the map.
        /// </summary>
        public double MouseX { get; private set; }

        /// <summary>
        /// Gets last known Y coordinate of mouse relative to upper left corner of the map.
        /// </summary>
        public double MouseY { get; private set; }

        /// <summary>
        /// Gets the map X of selected value (map pixel over which is the mouse).
        /// </summary>
        public int? SelectedValueX { get; private set; }

        /// <summary>
        /// Gets the map Y of selected value (map pixel over which is the mouse).
        /// </summary>
        public int? SelectedValueY { get; private set; }

        /// <summary>
        /// Gets the selected value (map pixel over which is the mouse).
        /// </summary>
        public int? SelectedValue { get; private set; }

        /// <summary>
        /// Gets a value indicating whether the mouse is over the map.
        /// </summary>
        /// <value>
        ///     <c>true</c> if the mouse is over the map; otherwise, <c>false</c>.
        /// </value>
        public bool IsMouseOverMap { get; private set; }

        /// <summary>
        /// Gets or sets a value indicating whether overlay is enabled.
        /// </summary>
        /// <value>
        ///     <c>true</c> if this instance is overlay enabled; otherwise, <c>false</c>.
        /// </value>
        [Persistent]
        public bool IsOverlayEnabled { get; set; }

        /// <summary>
        /// Gets or sets the last saved image path.
        /// </summary>
        /// <value>
        /// The last save image path.
        /// </value>
        [Persistent]
        public string LastSaveImagePath { get; set; }

        /// <summary>
        /// Gets the zoom-in command.
        /// </summary>
        public ICommand ZoomInCommand
        {
            get
            {
                if (this.zoomInCommand == null)
                {
                    this.zoomInCommand = new RelayCommand(param => this.ZoomIn(), p => this.DockManager.IsContentActive(this));
                }

                return this.zoomInCommand;
            }
        }

        /// <summary>
        /// Gets the zoom-out command.
        /// </summary>
        public ICommand ZoomOutCommand
        {
            get
            {
                if (this.zoomOutCommand == null)
                {
                    this.zoomOutCommand = new RelayCommand(param => this.ZoomOut());
                }

                return this.zoomOutCommand;
            }
        }

        /// <summary>
        /// Gets the save image command.
        /// </summary>
        public ICommand SaveImageCommand
        {
            get
            {
                if (this.saveImageCommand == null)
                {
                    this.saveImageCommand = new RelayCommand(
                        param => this.SaveImage(),
                        param => this.image.Source != null);
                }

                return this.saveImageCommand;
            }
        }

        /// <summary>
        /// Registers to the generator events.
        /// </summary>
        /// <param name="generator">The generator.</param>
        public void Register(IGenerator generator)
        {
            this.Generator = generator;

            generator.Finished += delegate
            {
                this.SelectedMap = generator.Maps[0];

                var adornerLayer = AdornerLayer.GetAdornerLayer(this.panel2D);
                adornerLayer.Remove(this.adorner);
            };

            // Gray out the 2D view while the generator is running
            generator.Started += delegate(object o, GenerationStartedEventArgs args)
            {
                if (args.HeaderOnly)
                {
                    return;
                }

                var adornerLayer = AdornerLayer.GetAdornerLayer(this.panel2D);
                var adorners = adornerLayer.GetAdorners(this.panel2D);

                if (adorners == null)
                {
                    return;
                }

                // The adorner may have been already added when the component was initialized
                if (adornerLayer.GetAdorners(this.panel2D) == null || !adorners.Contains(this.adorner))
                {
                    adornerLayer.Add(this.adorner);
                }
            };
        }

        /// <summary>
        /// Registers this sub-window to the docking manager.
        /// </summary>
        /// <param name="dockManager">The dock manager.</param>
        public void Register(IDockManager dockManager)
        {
            this.DockManager = dockManager;

            dockManager.AddAsDockableContent(this, "2D View", DockingLocation.Document);            
        }

        /// <summary>
        /// Registers global hotkeys.
        /// </summary>
        /// <param name="mainWindow">The main window.</param>
        public void Register(IMainWindow mainWindow)
        {
            mainWindow.RegisterInputGesture(
                new KeyGesture(Key.Add, ModifierKeys.None),
                this.ZoomInCommand);

            mainWindow.RegisterInputGesture(
                new KeyGesture(Key.OemPlus, ModifierKeys.None),
                this.ZoomInCommand);
            
            mainWindow.RegisterInputGesture(
                new KeyGesture(Key.Subtract, ModifierKeys.None),
                this.ZoomOutCommand);

            mainWindow.RegisterInputGesture(
                new KeyGesture(Key.OemMinus, ModifierKeys.None),
                this.ZoomOutCommand);
        }

        /// <summary>
        /// Registers cell in the status bar.
        /// </summary>
        /// <param name="statusBar">The status bar.</param>
        [OptionalRegistrator]
        public void Register(IStatusBar statusBar)
        {
            var valueBinding = new MultiBinding();
            valueBinding.StringFormat = "{0} x {1} [{2}]";
            valueBinding.Bindings.Add(
                new Binding
                {
                    Path = new PropertyPath("SelectedValueX"),
                    Source = this
                });

            valueBinding.Bindings.Add(
                new Binding
                {
                    Path = new PropertyPath("SelectedValueY"),
                    Source = this
                });

            valueBinding.Bindings.Add(
                new Binding
                {
                    Path = new PropertyPath("SelectedValue"),
                    Source = this
                });

            var statusBarEntry = new StatusBarEntry { Context = this.mouseOverContext, ValueBinding = valueBinding };
            statusBar.AddItem(statusBarEntry);
        }

        /// <summary>
        /// Zooms the map in by one step.
        /// </summary>
        public void ZoomIn()
        {
            if (this.ZoomFactor > View2D.MaximumZoomFactor)
            {
                return;
            }

            this.ZoomFactor *= View2D.ZoomInStep;

            this.image.Width = image.ActualWidth * View2D.ZoomInStep;
            this.image.Height = image.ActualHeight * View2D.ZoomInStep;

            // make sure the zooming is centered on mouse
            this.CenterOnMouse();

            this.ClampImagePosition();
        }

        /// <summary>
        /// Zooms the map out by one step.
        /// </summary>
        public void ZoomOut()
        {
            if (this.ZoomFactor < View2D.MinimumZoomFactor)
            {
                return;
            }

            this.ZoomFactor *= View2D.ZoomOutStep;

            this.image.Width = image.ActualWidth * View2D.ZoomOutStep;
            this.image.Height = image.ActualHeight * View2D.ZoomOutStep;

            this.CenterOnMouse();

            this.ClampImagePosition();
        }

        /// <summary>
        /// Saves the image to hard drive.
        /// </summary>
        public void SaveImage()
        {
            if (this.image.Source == null)
            {
                return;
            }

            try
            {
                var writer = new BitmapWriter();
                writer.IsFileDialogEnabled = true;
                this.LastSaveImagePath = writer.SaveBitmap((BitmapSource)this.image.Source, this.LastSaveImagePath);
            }
            catch (IOException e)
            {
                var errorMessage = new Message(e.Message, MessageType.Error);
                errorMessage.Send();
                return;
            }
            catch (OperationCanceledException)
            {
                return;
            }

            var message = new Message(@"Image """ + Path.GetFileName(this.LastSaveImagePath) + @""" saved.");
            message.Send();
        }

        /// <summary>
        /// Centers the image, so the image in in the middle of it. Zoom factor won't change.
        /// </summary>
        public void CenterOnMouse()
        {
            double mouseRatioX = this.image.ActualWidth / Mouse.GetPosition(this.image).X;
            double mouseRatioY = this.image.ActualHeight / Mouse.GetPosition(this.image).Y;

            double calculatedX = (double)this.image.GetValue(Canvas.LeftProperty) - ((this.image.Width - this.image.ActualWidth) / mouseRatioX);
            double calculatedY = (double)this.image.GetValue(Canvas.TopProperty) - ((this.image.Height - this.image.ActualHeight) / mouseRatioY);

            this.image.SetValue(Canvas.LeftProperty, calculatedX);            
            this.image.SetValue(Canvas.TopProperty, calculatedY);
        }

        /// <summary>
        /// Makes sure that at least <see cref="MinimumVisibleTip"/> is visible of the map on both axes.
        /// </summary>
        private void ClampImagePosition()
        {
            if (this.image.ActualWidth == 0 || this.image.ActualHeight == 0)
            {
                // The image is being erased (so a new image can be loaded), don't modify the position just yet.
                // This will be called a moment later once more.
                return;
            }

            // Left border
            if ((double)this.image.GetValue(Canvas.LeftProperty) + this.image.ActualWidth < View2D.MinimumVisibleTip)
            {
                this.image.SetValue(Canvas.LeftProperty, View2D.MinimumVisibleTip - this.image.ActualWidth);
            }

            // Right border
            if (this.canvas.ActualWidth - (double)this.image.GetValue(Canvas.LeftProperty) < View2D.MinimumVisibleTip)
            {
                this.image.SetValue(Canvas.LeftProperty, this.canvas.ActualWidth - View2D.MinimumVisibleTip);
            }

            // Top border
            if ((double)this.image.GetValue(Canvas.TopProperty) + this.image.ActualHeight < View2D.MinimumVisibleTip)
            {
                this.image.SetValue(Canvas.TopProperty, View2D.MinimumVisibleTip - this.image.ActualHeight);
            }
            
            // Bottom border
            if (this.canvas.ActualHeight - (double)this.image.GetValue(Canvas.TopProperty) < View2D.MinimumVisibleTip)
            {
                this.image.SetValue(Canvas.TopProperty, this.canvas.ActualHeight - View2D.MinimumVisibleTip);
            }
        }
    }
}
