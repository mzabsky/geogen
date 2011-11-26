namespace GeoGen.Studio.PlugIns.FullScreen
{
	using System;
	using System.Collections.Generic;
	using System.Linq;
	using System.Text;
	using System.Windows;
	using System.Windows.Documents;	
	using System.Windows.Input;
	using GeoGen.Studio.PlugInLoader;	
	using GeoGen.Studio.PlugIns.Interfaces;
	using GeoGen.Studio.PlugIns.ToolBars;
	using GeoGen.Studio.Utilities;	
	using GeoGen.Studio.Utilities.PlugInBase;	

	public class FullScreen: IPlugIn
	{
		private Window mainWindow;

		private ICommand toggleFullScreenCommand;

		private WindowState windowStateBackup;

		public FullScreen()
		{
			this.toggleFullScreenCommand = new RelayCommand(p => this.ToggleFullScreen());
		}

		public void Register(IMainWindow mainWindow){
			this.mainWindow = mainWindow.Window;

			this.mainWindow.Deactivated += delegate {
				if (this.mainWindow.WindowStyle == WindowStyle.None)
				{
					this.ToggleFullScreen();
				}
			};
		}

		public void Register(IMainWindowToolBar toolBar)
		{
			toolBar.AddItem(
				new ToolBarButton(new Run("label"), 0, this.toggleFullScreenCommand)
			);
		}

		public void Register(IMenuBar menuBar){
			string iconPathPrefix = "pack://application:,,,/GGenStudio.PlugIn.AvalonEditor;component/Images/Icons/";

			menuBar.AddMenu(
				new MenuEntry(
					header: "Edit",
					items: new MenuEntryObservableCollection()
					{
						new MenuEntry(
							header: "Full Screen",
							priority: 0,
							command: this.toggleFullScreenCommand,
							inputGestureText: "Ctrl+N"/*,
							icon: iconPathPrefix + "new.png"*/
						)
					}
				)
			);
		}


		private void ToggleFullScreen()
		{
			if (this.mainWindow.WindowStyle != WindowStyle.None)
			{
				this.windowStateBackup = this.mainWindow.WindowState;
				this.mainWindow.WindowState = WindowState.Maximized;
				this.mainWindow.WindowStyle = WindowStyle.None;

				Rect screenBounds = WpfScreen.GetScreen(this.mainWindow).DeviceBounds;
				this.mainWindow.Width = screenBounds.Width;
				this.mainWindow.Height = screenBounds.Height;
			}
			else
			{
				this.mainWindow.WindowState = this.windowStateBackup;
				this.mainWindow.WindowStyle = WindowStyle.SingleBorderWindow;
			}
		}


	}
}
