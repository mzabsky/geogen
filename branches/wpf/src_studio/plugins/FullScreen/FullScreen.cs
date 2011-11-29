namespace GeoGen.Studio.PlugIns.FullScreen
{
	using System;
	using System.Collections.Generic;
	using System.ComponentModel;
	using System.Windows;
	using System.Windows.Controls;
	using System.Windows.Data;
	using System.Windows.Input;
	using System.Windows.Media;	
	using GeoGen.Studio.PlugInLoader;
	using GeoGen.Studio.PlugIns.Interfaces;
	using GeoGen.Studio.PlugIns.MenuBars;
	using GeoGen.Studio.PlugIns.ToolBars;
	using GeoGen.Studio.Utilities;
	using GeoGen.Studio.Utilities.PlugInBase;
	using GeoGen.Studio.Utilities.Messaging;

	public sealed class FullScreen: ObjectBase, IPlugIn, INotifyPropertyChanged
	{
		private Window mainWindow;
		private List<Control> hideableBars = new List<Control>();

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

		public bool AreBarsShown {get; set;}

		public FullScreen()
		{
			this.toggleFullScreenCommand = new RelayCommand(p => this.ToggleFullScreen());			
			this.AreBarsShown = true;
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

			hideableBars.Add(toolBar.Control);
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

			hideableBars.Add(menuBar.Control);
		}

		public void Register(IStatusBar statusBar)
		{
			hideableBars.Add(statusBar.Control);
		}

		private void ToggleFullScreen()
		{
			if (this.mainWindow.WindowStyle != WindowStyle.None)
			{
				this.windowStateBackup = this.mainWindow.WindowState;
				this.mainWindow.WindowState = WindowState.Normal;
				
				this.mainWindow.WindowStyle = WindowStyle.None;
				this.mainWindow.WindowState = WindowState.Maximized;
				this.mainWindow.Topmost = true;
				this.mainWindow.Top = 0;
				this.mainWindow.Left = 0;

				this.HideBars();

				this.mainWindow.MouseMove += this.HandleMouseMoveWithBarsClosed;
			}
			else
			{
				this.mainWindow.ResizeMode = ResizeMode.CanResize;
				this.mainWindow.WindowState = this.windowStateBackup;
				this.mainWindow.WindowStyle = WindowStyle.SingleBorderWindow;
				this.mainWindow.Topmost = false;

				this.ShowBars();

				this.mainWindow.MouseMove -= this.HandleMouseMoveWithBarsOpen;
				this.mainWindow.MouseMove -= this.HandleMouseMoveWithBarsClosed;
			}

			GeoGen.Studio.App.Current.Dispatcher.BeginInvoke((Action)delegate{this.OnPropertyChanged("IsFullScreen");});
		}

		private void ShowBars(){
			if (this.AreBarsShown)
			{
				// Prevent the events from being hooked more than once
				return;
			}

			foreach (Control bar in this.hideableBars)
			{
				bar.Visibility = Visibility.Visible;
			}

			this.AreBarsShown = true;

			this.mainWindow.MouseMove -= this.HandleMouseMoveWithBarsClosed;
			this.mainWindow.MouseMove += this.HandleMouseMoveWithBarsOpen;
		}

		private void HideBars()
		{
			if (!this.AreBarsShown)
			{
				// Prevent the events from being hooked more than once
				return;
			}

			foreach (Control bar in this.hideableBars)
			{				
				bar.Visibility = Visibility.Collapsed;				
			}

			this.AreBarsShown = false;

			this.mainWindow.MouseMove -= this.HandleMouseMoveWithBarsOpen;
			this.mainWindow.MouseMove += this.HandleMouseMoveWithBarsClosed;
		}

		private void HandleMouseMoveWithBarsClosed(object sender, MouseEventArgs args){
			// Display the bars if the mouse is on top border of the screen
			if (args.MouseDevice.GetPosition(this.mainWindow).Y <= 1)
			{
				this.ShowBars();
			}
		}

		private void HandleMouseMoveWithBarsOpen(object sender, MouseEventArgs args)
		{
			// Check if the mouse is still above one of the bars
			bool barWasHit = false;
			foreach (Control bar in this.hideableBars)
			{
				// The bars are supposed to be rectangular, don't perform full hit test
				Point mousePoint = args.GetPosition(this.mainWindow);

				Point controlPoint = bar.TransformToAncestor(mainWindow).Transform(new Point(0, 0));

				barWasHit |= 
					mousePoint.X >= controlPoint.X &&
					mousePoint.Y >= controlPoint.Y &&
					mousePoint.X <= controlPoint.X + bar.ActualWidth &&
					mousePoint.Y <= controlPoint.Y + bar.ActualHeight;
			}

			// If the mouse is away from any bar, hide the bars
			if (!barWasHit)
			{
				this.HideBars();				
			}		
		}
	}
}
