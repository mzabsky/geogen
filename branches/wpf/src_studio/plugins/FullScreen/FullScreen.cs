namespace GeoGen.Studio.PlugIns.FullScreen
{
	using System;
	using System.Collections.Generic;
	using System.ComponentModel;
	using System.Windows;
	using System.Windows.Controls;
	using System.Windows.Data;
	using System.Windows.Input;
	using GeoGen.Studio.PlugInLoader;
	using GeoGen.Studio.PlugIns.Interfaces;
	using GeoGen.Studio.PlugIns.MenuBars;
	using GeoGen.Studio.PlugIns.ToolBars;
	using GeoGen.Studio.Utilities;
	using GeoGen.Studio.Utilities.PlugInBase;	

	public class FullScreen: ObjectBase, IPlugIn, INotifyPropertyChanged
	{
		private Window mainWindow;
		private List<Control> bars;

		private ICommand toggleFullScreenCommand;

		private WindowState windowStateBackup;

		public const string ICON_PATH_PREFIX = "pack://application:,,,/GeoGen.Studio.PlugIns.FullScreen;component/Images/Icons/";

		public bool IsFullScreen
		{
			get
			{
				return this.mainWindow.WindowStyle == WindowStyle.None;
			}
		}

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
				new ToolBarCheckableButton(
					icon: FullScreen.ICON_PATH_PREFIX  + "fullscreen.png", 
					priority: -10, 
					command: this.toggleFullScreenCommand, 
					toolTip: "Toggle full screen mode",					
					isCheckedBinding: new Binding("IsFullScreen"),
					dataContext: this
				)
			);

			//bars.Add(toolBar);
		}

		public void Register(IMenuBar menuBar){
			menuBar.AddMenu(
				new MenuEntry(
					header: "View",
					items: new MenuEntryObservableCollection()
					{
						new MenuEntry(
							header: "Full Screen",
							priority: -10,
							command: this.toggleFullScreenCommand,
							inputGestureText: "F11",
							icon: FullScreen.ICON_PATH_PREFIX  + "fullscreen.png",
							isCheckedBinding: new Binding("IsFullScreen"),
							dataContext: this,
							isCheckable: true
						)
					}
				)
			);

			//bars.Add(toolBar);
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

				foreach (Control bar in this.bars)
				{

				}
			}
			else
			{
				this.mainWindow.WindowState = this.windowStateBackup;
				this.mainWindow.WindowStyle = WindowStyle.SingleBorderWindow;
			}

			GeoGen.Studio.App.Current.Dispatcher.BeginInvoke((Action)delegate{this.OnPropertyChanged("IsFullScreen");});
		}
	}
}
