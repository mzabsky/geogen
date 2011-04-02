namespace GeoGen.Studio.PlugIns
{
    public class ToolBarControl: ToolBarEntry
    {
        public object Content { get; set; }

        public ToolBarControl() {}

        public ToolBarControl(object content, double priority = 0)
        {
            this.Content = content;
            this.Priority = priority;
        }
    }
}