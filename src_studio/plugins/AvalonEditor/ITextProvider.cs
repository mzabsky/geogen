using System.ComponentModel;
using GeoGen.Studio.PlugInLoader;

namespace GeoGen.Studio.PlugIns
{
    public interface ITextProvider : IPlugInInterface, INotifyPropertyChanged
    {
        string Text {get;}
    }
}
