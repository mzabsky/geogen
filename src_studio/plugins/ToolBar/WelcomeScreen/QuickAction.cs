using System.Windows.Input;
using System.Windows.Media;
using GeoGen.Studio.Utilities.Collections;

namespace GeoGen.Studio.PlugIns
{
    /// <summary>
    /// Represents one action in the <see cref="IQuickActionDisplay">quick action list</see>.
    /// </summary>
    public sealed class QuickAction: IPriority
    {
        /// <summary>
        /// Command associated with this action.
        /// </summary>
        /// <value>The command.</value>
        /// 
        public ICommand Command { get; set; }

        /// <summary>
        /// Small icon of the action.
        /// </summary>
        /// <value>The icon.</value>
        public ImageSource Icon { get; set; }

        /// <summary>
        /// Short user-friendly description of the action.
        /// </summary>
        /// <value>The label.</value>
        public string Label { get; set; }

        /// <summary>
        /// The tool tip with extended explanation of the action.
        /// </summary>
        /// <value>The tool tip.</value>
        public string ToolTip { get; set; }

        public double Priority { get; set; }
    }
}