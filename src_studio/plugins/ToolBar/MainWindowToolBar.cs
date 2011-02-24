using GeoGen.Studio.PlugInLoader;
using GeoGen.Studio.Utilities.Collections;

namespace GeoGen.Studio.PlugIns
{
    public class MainWindowToolBar: ToolBar, IMainWindowToolBar, IPlugIn, IPriority
    {
        public double Priority
        {
            get
            {
                return 0;
            }
        }

        /// <summary>
        /// Adds the toolbar into the main window.
        /// </summary>
        /// <param name="mainWindow">The main window.</param>
        public void Register(IMainWindow mainWindow)
        {
            mainWindow.TopBars.Add(this);
        }
    }
}