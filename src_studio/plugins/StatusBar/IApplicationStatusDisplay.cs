using GeoGen.Studio.Utilities.Context;
using GeoGen.Studio.PlugInLoader;

namespace GeoGen.Studio.PlugIns
{
    public interface IApplicationStatusDisplay: IPlugInInterface
    {
        void RegisterApplicationStatusContext(Context context);
    }
}
