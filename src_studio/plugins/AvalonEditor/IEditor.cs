using GeoGen.Studio.PlugInLoader;
using System.ComponentModel;

namespace GeoGen.Studio.PlugIns
{
    public interface IEditor: IPlugInInterface, INotifyPropertyChanged
    {
        string Text {get; set;}
    }
}
