using System.Windows;
using GeoGen.Studio.Utilities.Collections;

namespace GeoGen.Studio.PlugIns
{
    public class ToolBarSeparator: ToolBarEntry
    {
        static ToolBarSeparator()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(ToolBarSeparator),
                new FrameworkPropertyMetadata(typeof(ToolBarSeparator)));            
        }

        public ToolBarSeparator() {}

        public ToolBarSeparator(double priority) 
        {
            this.Priority = priority;
        }
    }
}
