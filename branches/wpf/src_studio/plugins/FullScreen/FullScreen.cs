namespace GeoGen.Studio.PlugIns
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

    /// <summary>
    /// Adds ability to switch the <see cref="IMainWindow">main window</see> to full screen.
    /// </summary>
    public sealed class FullScreen : ObjectBase, INotifyPropertyChanged
    {
        /// <summary>
        /// Path where icons for this plug-in are located.
        /// </summary>
        public const string IconPathPrefix = "pack://application:,,,/GeoGen.Studio.PlugIns.FullScreen;component/Images/Icons/";

        /// <summary>
        /// Collection of bars which can be hidden.
        /// </summary>
        private readonly List<Control> hideableBars = new List<Control>();

        /// <summary>
        /// The main window
        /// </summary>
        private Window mainWindow;

        /// <summary>
        /// Stores state of the window before it was switched to full screen.
        /// </summary>
        private WindowState windowStateBackup;

        /// <summary>
        /// Initializes a new instance of the <see cref="FullScreen"/> class.
        /// </summary>
        public FullScreen()
        {
            this.ToggleFullScreenCommand = new RelayCommand(p => this.ToggleFullScreen());            
            this.AreBarsShown = true;
        }

        /// <summary>
        /// Gets or sets the toggle full screen command.
        /// </summary>
        /// <value>
        /// The toggle full screen command.
        /// </value>
        public ICommand ToggleFullScreenCommand { get; set; }

        /// <summary>
        /// Gets a value indicating whether the window is in full screen mode.
        /// </summary>
        /// <value>
        ///     <c>true</c> if this instance is full screen; otherwise, <c>false</c>.
        /// </value>
        public bool IsFullScreen
        {
            get
            {
                return this.mainWindow.WindowStyle == WindowStyle.None;
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether the main window bars are currently displayed.
        /// </summary>
        /// <value>
        ///   <c>true</c> if [are bars shown]; otherwise, <c>false</c>.
        /// </value>
        public bool AreBarsShown { get; set; }

        /// <summary>
        /// Enables the main window to be hidden.
        /// </summary>
        /// <param name="mainWindow">The main window.</param>
        public void Register(IMainWindow mainWindow)
        {
            this.mainWindow = mainWindow.Window;

            this.mainWindow.Deactivated += delegate
            {
                if (this.mainWindow.WindowStyle == WindowStyle.None)
                {
                    this.ToggleFullScreen();
                }                
            };            
        }

        /// <summary>
        /// Adds the Full Screen button to the main window toolbar.
        /// </summary>
        /// <param name="toolBar">The tool bar.</param>
        [OptionalRegistrator]
        public void Register(IMainWindowToolBar toolBar)
        {
            toolBar.AddItem(
                new ToolBarCheckableButton(
                    icon: FullScreen.IconPathPrefix  + "fullscreen.png", 
                    priority: -10, 
                    command: this.ToggleFullScreenCommand, 
                    toolTip: "Toggle full screen mode",                    
                    isCheckedBinding: new Binding("IsFullScreen"),
                    dataContext: this));

            this.hideableBars.Add(toolBar.Control);
        }

        /// <summary>
        /// Registers the main window menu bar.
        /// </summary>
        /// <param name="menuBar">The menu bar.</param>
        [OptionalRegistrator]
        public void Register(IMenuBar menuBar)
        {
            // TODO: IMainWindowMenuBar!
            menuBar.AddMenu(
                new MenuEntry(
                    header: "View",
                    items: new MenuEntryObservableCollection
                    {
                        new MenuEntry(
                            header: "Full Screen",
                            priority: -10,
                            command: this.ToggleFullScreenCommand,
                            inputGestureText: "F11",
                            icon: FullScreen.IconPathPrefix  + "fullscreen.png",
                            isCheckedBinding: new Binding("IsFullScreen"),
                            dataContext: this,
                            isCheckable: true)
                    }));

            this.hideableBars.Add(menuBar.Control);
        }

        /// <summary>
        /// Registers the status bar to be hidden when switching to full screen.
        /// </summary>
        /// <param name="statusBar">The status bar.</param>
        [OptionalRegistrator]
        public void Register(IStatusBar statusBar)
        {
            this.hideableBars.Add(statusBar.Control);
        }

        /// <summary>
        /// Toggles the main window to/from full screen mode.
        /// </summary>
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

            Application.Current.Dispatcher.BeginInvoke((Action)delegate
            {
                this.OnPropertyChanged("IsFullScreen");
            });
        }

        /// <summary>
        /// Unhides main window bars.
        /// </summary>
        private void ShowBars()
        {
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

        /// <summary>
        /// Hides the main window bars, which are registered to be hidden when switching to full screen mode.
        /// </summary>
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

        /// <summary>
        /// Shows the bars if the mouse is hovering on top of the screen.
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="args">The <see cref="System.Windows.Input.MouseEventArgs"/> instance containing the event data.</param>
        private void HandleMouseMoveWithBarsClosed(object sender, MouseEventArgs args)
        {
            // Display the bars if the mouse is on top border of the screen
            if (args.MouseDevice.GetPosition(this.mainWindow).Y <= 1)
            {
                this.ShowBars();
            }
        }

        /// <summary>
        /// Hides the bars which were shown by <see cref="HandleMouseMoveWithBarsClosed"/>.
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="args">The <see cref="System.Windows.Input.MouseEventArgs"/> instance containing the event data.</param>
        private void HandleMouseMoveWithBarsOpen(object sender, MouseEventArgs args)
        {
            // Check if the mouse is still above one of the bars
            bool barWasHit = false;
            foreach (Control bar in this.hideableBars)
            {
                // The bars are supposed to be rectangular, don't perform full hit test
                Point mousePoint = args.GetPosition(this.mainWindow);

                Point controlPoint = bar.TransformToAncestor(this.mainWindow).Transform(new Point(0, 0));

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
