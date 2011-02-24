using System.Windows.Controls;
using System.Windows.Controls.Primitives;

namespace GeoGen.Studio.PlugIns
{
    public interface IToolBar
    {
        void AddItem(Separator item);
        void AddItem(Button item);
        void AddItem(ToggleButton item);
        void AddItem(ToolBarEntry item);
    }
}
