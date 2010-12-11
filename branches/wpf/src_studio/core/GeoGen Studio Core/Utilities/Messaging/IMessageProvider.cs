using GeoGen.Studio.PlugInLoader;

namespace GeoGen.Studio.Utilities.Messaging
{
    public interface IMessageProvider: IPlugInInterface
    {
        event MessageThrownEventHandler MessageThrown;
    }
}
