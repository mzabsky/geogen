using GeoGen.Studio.PlugInLoader;
using System.Windows.Input;
using GeoGen.Studio.Utilities.Collections;

namespace GeoGen.Studio.PlugIns
{
    /// <summary>
    /// Main window for the application, that can place any number of horizontal bars on top and bottom side of the window and one control as its content.
    /// </summary>
    public interface IMainWindow: IPlugInInterface
    {
        /// <summary>
        /// Main content of the window.
        /// </summary>
        /// <value>The content.</value>
        object Content{get; set;}

        /// <summary>
        /// Collection of horizontal bars (<see cref="System.Windows.Controls.Control">controls</see> implementing <see cref="IPriority"/>) that will be placed on top side of the window. The bars will be sorted by their <see cref="IPriority">priority</see>.
        /// </summary>
        PriorityObservableCollection TopBars { get; set; }

        /// <summary>
        /// Collection of horizontal bars (<see cref="System.Windows.Controls.Control">controls</see> implementing <see cref="IPriority"/>) that will be placed on bottom side of the window. The bars will be sorted by their <see cref="IPriority">priority</see>.
        /// </summary>
        PriorityObservableCollection BottomBars { get; set; }

        /// <summary>
        /// Registers a window-wide <see cref="InputGesture"/>.
        /// </summary>
        /// <param name="gesture">The gesture.</param>
        /// <param name="command">The command.</param>
        void RegisterInputGesture(InputGesture gesture, ICommand command);
    }
}
