namespace GeoGen.Studio.PlugIns.Interfaces
{
	using System.Windows.Controls;
	using System.Windows.Controls.Primitives;
	using GeoGen.Studio.PlugIns.ToolBars;

    /// <summary>
    /// Base for other toolbar-related interfaces.
    /// </summary>
    public interface IToolBar
    {
        /// <summary>
        /// Adds a WPF separator to the toolbar.
        /// </summary>
        /// <param name="item">The separator.</param>
        void AddItem(Separator item);

        /// <summary>
        /// Adds a common WPF button to the toolbar.
        /// </summary>
        /// <param name="item">The button.</param>
        void AddItem(Button item);

        /// <summary>
        /// Adds a WPF toggle button to the toolbar.
        /// </summary>
        /// <param name="item">The button.</param>
        void AddItem(ToggleButton item);

        /// <summary>
        /// Adds a <see cref="ToolBarEntry"/> to the toolbar.
        /// </summary>
        /// <param name="item">The item.</param>
        void AddItem(ToolBarEntry item);
    }
}
