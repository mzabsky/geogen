namespace GeoGen.Studio.PlugIns
{
    using System.ComponentModel;

    using GeoGen.Studio.PlugInLoader;

    /// <summary>
    /// Application window full screen mode plug-in interface.
    /// </summary>
    public interface IFullScreen : INotifyPropertyChanged, IPlugIn
    {
        /// <summary>
        /// Gets a value indicating whether the window is in full screen mode.
        /// </summary>
        /// <value>
        ///     <c>true</c> if this instance is in full screen mode; otherwise, <c>false</c>.
        /// </value>
        bool IsFullScreen { get; }

        /// <summary>
        /// Switches the window to fullscreen mode.
        /// </summary>
        void EnableFullScreen();

        /// <summary>
        /// Switches the window from fullscreen mode.
        /// </summary>
        void DisableFullScreen();
    }
}