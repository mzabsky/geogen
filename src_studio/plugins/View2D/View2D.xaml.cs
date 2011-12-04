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

	public partial class View2D : INotifyPropertyChanged
	{
		private Adorner adorner;

		private bool scrolling;
		private double lastDragX;
		private double lastDragY;
		private double dragPointX;
		private double dragPointY;

		private const double MinimumVisibleTip = 20;

		private ICommand zoomInCommand;
		private ICommand zoomOutCommand;
		private ICommand resetZoomCommand;
		private ICommand saveImageCommand;

		private Overlay selectedOverlay;

		private Context mouseOverContext = new Context("View 2D");

		public IDockManager DockManager { get; private set; }
		
		public IGenerator Generator { get; private set; }

		public HeightData SelectedMap { get; set; }

		[Persistent(UseEmptyInstanceAsDefault = true)]
		public Overlay SelectedOverlay {
			get
			{
				return selectedOverlay;
			}
			set
			{
				this.selectedOverlay = value;
			}
		}

		public double ZoomFactor { get; set; }

		public double MouseX { get; private set; }

		public double MouseY { get; private set; }

		public int? SelectedValueX { get; private set; }

		public int? SelectedValueY { get; private set; }

		public int? SelectedValue { get; private set; }

		public bool IsMouseOverMap { get; private set; }

		[Persistent]
		public string LastSaveImagePath {get; set;}

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

		public View2D()
		{
			this.ZoomFactor = 1;

			InitializeComponent();
			MainConfig.Register(this);

			this.adorner = new GeoGen.Studio.UI.BusyAdorner(this.panel2D);
			var adornerLayer = AdornerLayer.GetAdornerLayer(this.panel2D);
			adornerLayer.Add(this.adorner);

			var items = toolbar.Items;

			this.image.MouseDown += delegate(object sender, MouseButtonEventArgs args)
				{
					if (args.ChangedButton != MouseButton.Left)
					{
						return;
					}

					this.dragPointX = Mouse.GetPosition(this.image).X;
					this.dragPointY = Mouse.GetPosition(this.image).Y;
					this.lastDragX = (double)this.image.GetValue(Canvas.LeftProperty) + this.dragPointX;
					this.lastDragY = (double)this.image.GetValue(Canvas.TopProperty) + this.dragPointY;

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

			this.image.MouseMove += delegate(object sender, MouseEventArgs args)
				{
					if (this.image.IsMouseCaptured)
					{
						this.image.SetValue(
							Canvas.LeftProperty,
							(double)image.GetValue(Canvas.LeftProperty) - (this.lastDragX - Mouse.GetPosition(this.canvas).X));
						this.image.SetValue(
							Canvas.TopProperty,
							(double)image.GetValue(Canvas.TopProperty) - (this.lastDragY - Mouse.GetPosition(this.canvas).Y));

						this.lastDragX = (double)this.image.GetValue(Canvas.LeftProperty) + this.dragPointX;
							// Mouse.GetPosition(this.canvas).X;
						this.lastDragY = (double)this.image.GetValue(Canvas.TopProperty) + this.dragPointY;

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

			this.image.SizeChanged += delegate { this.ClampImagePosition(); };
		}

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
				if(args.HeaderOnly) {
					return;
				}

				var adornerLayer = AdornerLayer.GetAdornerLayer(this.panel2D);

				// The adorner may have been already added when the component was initialized
				if(adornerLayer.GetAdorners(this.panel2D) == null || !adornerLayer.GetAdorners(this.panel2D).Contains(this.adorner)){
					adornerLayer.Add(this.adorner);
				}
			};
		}

		public void Register(IDockManager dockManager)
		{
			this.DockManager = dockManager;

			dockManager.AddAsDockableContent(this, "2D View", DockingLocation.Document);            
		}

		public void Register(IMainWindow mainWindow)
		{
			mainWindow.RegisterInputGesture(
				new KeyGesture(Key.Add, ModifierKeys.None),
				this.ZoomInCommand				
			);

			mainWindow.RegisterInputGesture(
				new KeyGesture(Key.OemPlus, ModifierKeys.None),
				this.ZoomInCommand
			);
			
			mainWindow.RegisterInputGesture(
				new KeyGesture(Key.Subtract, ModifierKeys.None),
				this.ZoomOutCommand
			);

			mainWindow.RegisterInputGesture(
				new KeyGesture(Key.OemMinus, ModifierKeys.None),
				this.ZoomOutCommand
			);
		}

		[OptionalRegistrator]
		public void Register(IStatusBar statusBar)
		{
			MultiBinding valueBinding = new MultiBinding();
			valueBinding.StringFormat = "{0} x {1} [{2}]";
			valueBinding.Bindings.Add(
				new Binding
				{
					Path = new PropertyPath("SelectedValueX"),
					Source = this
				}
			);
			valueBinding.Bindings.Add(
				new Binding
				{
					Path = new PropertyPath("SelectedValueY"),
					Source = this
				}
			);
			valueBinding.Bindings.Add(
				new Binding
				{
					Path = new PropertyPath("SelectedValue"),
					Source = this
				}
			);

			statusBar.AddItem(
				new StatusBarEntry
				{
					Context = this.mouseOverContext,
					ValueBinding = valueBinding
				}
			);
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
			if (this.image.Source == null)
			{
				return;
			}

			try
			{
				BitmapWriter writer = new BitmapWriter();
				writer.IsFileDialogEnabled = true;
				this.LastSaveImagePath = writer.SaveBitmap((BitmapSource)this.image.Source, this.LastSaveImagePath);
			}
			catch (IOException e)
			{
				Message errorMessage = new Message(e.Message, MessageType.Error);
				Messenger.ThrowMessage(errorMessage);
				return;
			}
			catch (OperationCanceledException)
			{
				return;
			}

			Message message = new Message(@"Image """ + Path.GetFileName(this.LastSaveImagePath) + @""" saved.", MessageType.Message);
			Messenger.ThrowMessage(message);
		}

		private void ClampImagePosition(){
			if (this.image.ActualWidth == 0 || this.image.ActualHeight == 0)
			{
				// The image is being erased (so a new image can be loaded), don't modify the position just yet.
				// This will be called a moment later once more.
				return;
			}

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
