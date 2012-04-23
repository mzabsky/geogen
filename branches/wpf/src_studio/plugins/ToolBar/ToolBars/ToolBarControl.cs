namespace GeoGen.Studio.PlugIns.ToolBars
{
    using System.Windows;

    public class ToolBarControl: ToolBarEntry
    {
        static ToolBarControl()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(ToolBarControl),
                new FrameworkPropertyMetadata(typeof(ToolBarControl)));            
        }

        public ToolBarControl() {}

        public ToolBarControl(object content, double priority = 0)
        {
            this.Content = content;
            this.Priority = priority;
        }
    }
}