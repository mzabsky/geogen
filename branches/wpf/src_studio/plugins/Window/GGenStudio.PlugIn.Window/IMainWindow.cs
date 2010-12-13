using System.Collections.ObjectModel;
using GeoGen.Studio.PlugInLoader;
using System.Windows.Input;
using GeoGen.Studio.Utilities.Collections;

namespace GeoGen.Studio.PlugIns
{
    public interface IMainWindow: IPlugInInterface
    {
        object Content{get; set;}
        PriorityObservableCollection TopBars { get; set; }
        PriorityObservableCollection BottomBars { get; set; }

        void RegisterInputGesture(InputGesture gesture, ICommand command);
    }
}
