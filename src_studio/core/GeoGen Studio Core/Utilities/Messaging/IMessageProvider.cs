using GeoGen.Studio.PlugInLoader;
using System.Windows;

namespace GeoGen.Studio.Utilities.Messaging
{
    public interface IMessageProvider: IPlugInInterface
    {
        event MessageThrownEventHandler MessageThrown;
    }
}
