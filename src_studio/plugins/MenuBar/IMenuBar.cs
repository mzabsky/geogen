using GeoGen.Studio.PlugInLoader;
using System.Windows.Controls;

namespace GeoGen.Studio.PlugIns
{
    public interface IMenuBar: IPlugInInterface
    {
        void AddMenu(MenuItem menu);
        void AddMenu(MenuEntry menu);
    }
}
