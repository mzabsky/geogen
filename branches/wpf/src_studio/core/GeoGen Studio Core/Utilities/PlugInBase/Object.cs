using System.ComponentModel;
using GeoGen.Studio.PlugInLoader;

namespace GeoGen.Studio.Utilities.PlugInBase
{
    /// <summary>
    /// Base class for plug-ins that are not based on any class (other than <see cref="object"/>). Provides <see cref="INotifyPropertyChanged"/> implementation.
    /// </summary>
    public class ObservableObject : IPlugIn
    {
    }
}
