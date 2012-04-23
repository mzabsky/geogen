namespace GeoGen.Studio.PlugIns
{
    using System.Windows.Input;
    using System.Windows.Media;
    using GeoGen.Studio.Utilities.Collections;

    /// <summary>
    /// Represents one action in the <see cref="IQuickActionDisplay">quick action list</see>.
    /// </summary>
    public sealed class QuickAction : IPriority
    {
        /// <summary>
        /// Gets or sets the command associated with this action.
        /// </summary>
        /// <value>The command.</value>
        public ICommand Command { get; set; }

        /// <summary>
        /// Gets or sets small icon of the action.
        /// </summary>
        /// <value>The icon.</value>
        public ImageSource Icon { get; set; }

        /// <summary>
        /// Gets or sets short user-friendly description of the action.
        /// </summary>
        /// <value>The label.</value>
        public string Label { get; set; }

        /// <summary>
        /// Gets or sets tool tip with extended explanation of the action.
        /// </summary>
        /// <value>The tool tip.</value>
        public string ToolTip { get; set; }

        /// <summary>
        /// Gets or sets priority used to determine position of this action in collection of actions.
        /// </summary>
        public double Priority { get; set; }
    }
}