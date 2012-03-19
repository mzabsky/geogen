namespace GeoGen.Studio.PlugIns.Viewers
{
	using System.ComponentModel;
	using System.Windows;
	using GeoGen.Studio.Utilities.Persistence;
	using SlimDX.Wpf;

	/// <summary>
	/// Interaction logic for UserControl1.xaml
	/// </summary>
	public sealed partial class View3D : INotifyPropertyChanged
	{
		private DataModel dataModel;
		private RenderEngine renderEngine;

		public IDockManager DockManager { get; private set; }

		public IGenerator Generator { get; private set; }

		public HeightData SelectedMap { get; set; }

		[Persistent(UseEmptyInstanceAsDefault = true)]
		public Overlay SelectedOverlay { get; set; }        

		public View3D()
		{
			InitializeComponent();

			Initialize3D();
		}

		private void Initialize3D(){
			viewport.InitializeComponent();
			
			if(viewport.DirectXStatus != DirectXStatus.Available)
			{
				//MouseDown -= MouseDownHandler;

				if (viewport.DirectXStatus == DirectXStatus.Unavailable_RemoteSession)
				{
					MessageBox.Show("DirectX not supported when using Remote Desktop", "Error intializing DirectX");
					System.Environment.Exit(1);
				}
				else if (viewport.DirectXStatus == DirectXStatus.Unavailable_LowTier)
				{
					MessageBox.Show("Insufficient graphics acceleration on this machine", "Error intializing DirectX");
					System.Environment.Exit(1);
				}
				else if (viewport.DirectXStatus == DirectXStatus.Unavailable_MissingDirectX)
				{
					MessageBox.Show("DirectX libraries are missing or need to be updated", "Error intializing DirectX");
					System.Environment.Exit(1);
				}
				else
				{
					MessageBox.Show("Unable to start DirectX (reason unknown)", "Error intializing DirectX");
					System.Environment.Exit(1);
				}
				return;
			}

			this.dataModel = new DataModel(viewport, "lizard", System.Drawing.Color.Blue);
			this.renderEngine = new RenderEngine(this.dataModel);            
			this.renderEngine.EnableRenderCondition = delegate
			{
				return this.DockManager.IsContentActive(this);
			};

			this.Loaded += delegate
			{
				this.viewport.SetRenderEngine(this.renderEngine);
				//this.viewport.Device.				
				// [1236] Direct3D9: (ERROR) :Invalid value for BackBufferFormat. ValidatePresentParameters fails.

			};


			/*Window.GetWindow(this).Closed += delegate
			{
				this.viewport.Shutdown();
			};*/
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
			this.DockManager = dockManager;

			dockManager.AddAsDockableContent(this, "3D View", DockingLocation.Document);            
		}
	}
}
