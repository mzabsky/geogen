using System.Collections.ObjectModel;

namespace GeoGen_Studio
{
    public interface IMainWindow
    {
        object Content{get; set;}
        ObservableCollection<object> TopBars { get; set; }
        ObservableCollection<object> BottomBars { get; set; }
    }
}
