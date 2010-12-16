using GeoGen.Studio.PlugInLoader;

namespace GeoGen.Studio.PlugIns
{
    public interface IStatusBar: IPlugInInterface
    {
        void AddItem(StatusBarEntry item);
    }
}
