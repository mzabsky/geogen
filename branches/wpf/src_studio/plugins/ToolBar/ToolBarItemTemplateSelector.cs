using System.Windows;
using System.Windows.Controls;

namespace GeoGen.Studio.PlugIns
{
    internal class ToolBarItemTemplateSelector : DataTemplateSelector
    {
        public DataTemplate ToolBarCheckableButton { get; set; }
        public DataTemplate ToolBarButton { get; set; }
        public DataTemplate ToolBarSeparator { get; set; }

        public override DataTemplate SelectTemplate(object item, DependencyObject container)
        {
            if (item is ToolBarCheckableButton)
            {
                return ToolBarCheckableButton;
            }
            else if (item is ToolBarButton)
            {
                return ToolBarButton;
            }
            else if (item is ToolBarSeparator)
            {
                return ToolBarSeparator;
            }
            return base.SelectTemplate(item, container);
        }

        public ToolBarItemTemplateSelector()
        {
            this.ToolBarButton = null;            
        }
    }

}
