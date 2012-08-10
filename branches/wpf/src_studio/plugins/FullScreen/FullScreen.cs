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
    /// Lists sources which could cause bars to be shown in full screen mode.
    /// </summary>
    public enum BarsShowSources
    {
        Keyboard,
        Mouse
    }

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

            this.mainWindow.KeyDown += delegate(object o, KeyEventArgs args)
            {
                if (this.IsFullScreen)
                {
                    if (this.ShouldBarsToggleByKeyboard(args))
                    {
                        this.ShowBars();

                        args.Handled = true;
                    }
                }
            };

            this.mainWindow.KeyUp += delegate(object o, KeyEventArgs args)
            {
                if (this.IsFullScreen)
                {
                    if (this.ShouldBarsToggleByKeyboard(args))
                    {
                        this.HideBars();
                    }

                    if (this.ShouldFullscreenExitByKeyboard(args))
                    {

                        this.ToggleFullScreen();
                    }
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
                    priority: -8,
                    group: "display",
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
        /// Determines whether a key change involved the toggle bars key and whether toggling bars by keyboard is enabled.
        /// </summary>
        /// <param name="args">The <see cref="System.Windows.Input.KeyEventArgs"/> instance containing the keyboard event data.</param>
        /// <returns>
        ///   <c>true</c> if the change invol; otherwise, <c>false</c>.
        /// </returns>
        private bool ShouldBarsToggleByKeyboard(KeyEventArgs args)
        {
            return args.SystemKey == Key.LeftAlt || args.SystemKey == Key.RightAlt;
        }

        /// <summary>
        /// Determines whether the fullscreen should exit by keyboard.
        /// </summary>
        /// <param name="args">The <see cref="System.Windows.Input.KeyEventArgs"/> instance containing the keyboard event data.</param>
        /// <returns></returns>
        private bool ShouldFullscreenExitByKeyboard(KeyEventArgs args)
        {
            return args.Key == Key.Escape;
        }

        /// <summary>
        /// Toggles the main window to/from full screen mode.
        /// </summary>
        private void ToggleFullScreen()
        {
            if (this.mainWindow.WindowStyle != WindowStyle.None)
            {
                this.EnableFullScreen();
            }
            else
            {
                this.DisableFullScreen();
            }
        }

        /// <summary>
        /// Switches the window to fullscreen mode.
        /// </summary>
        private void EnableFullScreen()
        {
            this.windowStateBackup = this.mainWindow.WindowState;
            this.mainWindow.WindowState = WindowState.Normal;

            this.mainWindow.WindowStyle = WindowStyle.None;
            this.mainWindow.WindowState = WindowState.Maximized;
            this.mainWindow.Topmost = true;
            this.mainWindow.Top = 0;
            this.mainWindow.Left = 0;

            this.HideBars();

            Application.Current.Dispatcher.BeginInvoke((Action)delegate
            {
                this.OnPropertyChanged("IsFullScreen");
            });
        }

        /// <summary>
        /// Switches the window from fullscreen mode.
        /// </summary>
        private void DisableFullScreen()
        {
            this.mainWindow.ResizeMode = ResizeMode.CanResize;
            this.mainWindow.WindowState = this.windowStateBackup;
            this.mainWindow.WindowStyle = WindowStyle.SingleBorderWindow;
            this.mainWindow.Topmost = false;

            this.ShowBars();

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
            foreach (Control bar in this.hideableBars)
            {
                bar.Visibility = Visibility.Visible;
            }
        }

        /// <summary>
        /// Hides the main window bars, which are registered to be hidden when switching to full screen mode.
        /// </summary>
        private void HideBars()
        {
            foreach (Control bar in this.hideableBars)
            {                
                bar.Visibility = Visibility.Collapsed;                
            }
        }
    }
}
