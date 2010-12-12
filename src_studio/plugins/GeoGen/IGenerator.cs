using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Collections.Generic;
using GeoGen.Studio;
using GeoGen.Studio.PlugInLoader;

namespace GeoGen.Studio.PlugIns
{
    public interface IGenerator : IPlugInInterface, INotifyPropertyChanged
    {
        event GenerationFailedEventHandler GenerationFailed;
        event GenerationFinishedEventHandler GenerationFinished;
        event EventHandler HeaderLoaded;
        
        bool IsReady { get;}
        double Progress { get; }
        ObservableCollection<HeightData> Maps { get; }
        ObservableCollection<ScriptArg> Args { get; }

        void Start(string script, bool headerOnly = false, IEnumerable<uint> parametersOverride = null);
        void Abort();
    }
}
