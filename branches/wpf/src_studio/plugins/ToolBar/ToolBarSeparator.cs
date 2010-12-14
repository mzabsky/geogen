using GeoGen.Studio.Utilities.Collections;

namespace GeoGen.Studio.PlugIns
{
    public class ToolBarSeparator: ToolBarEntry
    {
        public ToolBarSeparator() {}

        public ToolBarSeparator(double priority) 
        {
            this.Priority = priority;
        }
    }
}
