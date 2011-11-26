namespace GeoGen.Studio.PlugIns
{
	using System;
	using System.Collections.Generic;
	using System.Collections.ObjectModel;
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
	using GeoGen.Studio.PlugInLoader;
	using GeoGen.Studio.PlugIns.Interfaces;

	[PlugIn(VisibleInList = false)]
	public partial class DockManager : UserControl, IPlugIn, IDockManager
	{
		Dictionary<object, AvalonDock.ManagedContent> registeredContents = new Dictionary<object, AvalonDock.ManagedContent>();        

		public DockManager()
		{
			this.InitializeComponent();
			this.dockManager.RestoringLayout = true;

			Loader.Ready += delegate
			{
				this.dockManager.RestoringLayout = false;
			};
		}

		public void Register(IMainWindow mainWindow)
		{
			mainWindow.Content = this;
		}

		// TODO
		// - zrusit focus (udelat podle priority)

		public bool AddAsDocumentContent(object content, string title, bool focus = false){
			if(registeredContents.ContainsKey(content))
			{
				AvalonDock.ManagedContent existingContent = registeredContents[content];
				
				if (!existingContent.IsArrangeValid) existingContent.Show();
				existingContent.Activate();

				return false;
			}

			AvalonDock.DocumentContent avalonContent = new AvalonDock.DocumentContent();
			avalonContent.Content = content;
			avalonContent.Title = title;
			avalonContent.Show(this.dockManager);

			if(focus)
			{
				avalonContent.Activate();
			}

			registeredContents.Add(content, avalonContent);

			return true;
		}

		public bool AddAsDockableContent(object content, string title, DockingLocation preferredLocation)
		{
			if (registeredContents.ContainsKey(content))
			{
				dynamic existingContent = registeredContents[content];

				if (!existingContent.IsArrangeValid) existingContent.Show();
				existingContent.Activate();

				return false;
			}

			AvalonDock.DockableContent avalonContent = new AvalonDock.DockableContent();
			avalonContent.Content = content;
			avalonContent.Title = title;
			avalonContent.SavedStateAndPosition = this.GetDockingStateByLocation(preferredLocation) as AvalonDock.DockableContentStateAndPosition;
			avalonContent.SavedStateAndPosition.ContainerPane.Items.Add(avalonContent);
			//avalonContent.ContainerPane = avalonContent.SavedStateAndPosition.ContainerPane;
			//avalonContent.Show(this.dockManager);			

			if (avalonContent.ContainerPane is AvalonDock.DocumentPane)
			{
				avalonContent.ShowAsDocument(this.dockManager);
			}
			else
			{
				avalonContent.Show();
			}

			registeredContents.Add(content, avalonContent);

			return true;
		}

		public bool AddAsFloatableWindow(object content, string title, Size size, bool dockable = true)
		{
			if (registeredContents.ContainsKey(content))
			{
				AvalonDock.ManagedContent existingContent = registeredContents[content];

				// TODO: fix this if (!existingContent.IsArrangeValid) existingContent.ShowAsFloatingWindow();
				existingContent.Activate();

				return false;
			}

			AvalonDock.DockableContent avalonContent = new AvalonDock.DockableContent();
			avalonContent.Content = content;
			avalonContent.Title = title;
			avalonContent.Show(this.dockManager);

			if(size != null)
			{
				avalonContent.FloatingWindowSize = size;
			}

			avalonContent.ShowAsFloatingWindow(this.dockManager, dockable);

			registeredContents.Add(content, avalonContent);

			return true;
		}

		public void Activate(object content)
		{
			if (!registeredContents.ContainsKey(content))
			{
				throw new InvalidOperationException("Passed content is not registered with this dock manager.");
			}
			
			dynamic avalonContent = registeredContents[content];
			avalonContent.Activate();			
		}

		public object GetDockingState(object content) 
		{
			throw new NotImplementedException(); 
		}

		public object GetDockingStateByLocation(DockingLocation location) {
			AvalonDock.Pane pane;
			AvalonDock.DockableContentState state;
			switch (location)
			{
				case DockingLocation.LeftTop:
					pane = this.leftTopPane;
					state = AvalonDock.DockableContentState.Docked;
					break;
				case DockingLocation.LeftBottom:
					pane = this.leftBottomPane;
					state = AvalonDock.DockableContentState.Docked;
					break;
				case DockingLocation.RightTop:
					pane = this.rightTopPane;
					state = AvalonDock.DockableContentState.Docked;
					break;
				case DockingLocation.RightBottom:
					pane = this.rightBottomPane;
					state = AvalonDock.DockableContentState.Docked;
					break;
				case DockingLocation.Top:
					pane = this.topPane;
					state = AvalonDock.DockableContentState.Docked;
					break;
				case DockingLocation.Bottom:
					pane = this.bottomPane;
					state = AvalonDock.DockableContentState.Docked;
					break;
				case DockingLocation.Document:
					pane = this.documentPane;
					state = AvalonDock.DockableContentState.Document;
					break;
				default:
					throw new NotImplementedException();
			}

			return new AvalonDock.DockableContentStateAndPosition(	
				containerPane: pane,
				childIndex: 0,
				width: Math.Max(pane.ActualWidth, 100.0),
				height: Math.Max(pane.ActualHeight, 100.0),
				anchor: AvalonDock.AnchorStyle.None,
				state: state				
			);
		}

		public bool IsContentActive(object content)
		{
			if (!registeredContents.ContainsKey(content))
			{
				throw new InvalidOperationException("Passed content is not registered with this dock manager.");
			}

			AvalonDock.ManagedContent avalonContent = registeredContents[content];
			return avalonContent.IsActiveContent;			
		}
	}
}
