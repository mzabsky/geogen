using System.Collections.ObjectModel;
using GeoGen.Studio.PlugInLoader;
using System.Windows.Input;

namespace GeoGen.Studio.PlugIns
{
    public interface IMainWindow: IPlugInInterface
    {
        object Content{get; set;}
        ObservableCollection<object> TopBars { get; set; }
        ObservableCollection<object> BottomBars { get; set; }

        void RegisterInputGesture(InputGesture gesture, ICommand command);
    }
}
