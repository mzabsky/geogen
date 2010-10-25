using GeoGen.Studio.PlugInLoader;
using System.Windows;

namespace GeoGen.Studio.Utilities.Messaging
{
    interface IMessageProvider: IPlugInInterface
    {
        event RoutedEventHandler MessageThrown;
    }
}
