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
        event GenerationStartingEventHandler Starting;
        event GenerationStartedEventHandler Started;
        event EventHandler HeaderLoaded;
        event EventHandler Aborted;
        event GenerationFailedEventHandler Failed;
        event GenerationFinishedEventHandler Finished;

        /// <summary>
        /// Indicates whether the generator is ready to accept requests to run scripts.
        /// </summary>
        /// <value><c>true</c> if the generator is ready; otherwise, <c>false</c>.</value>
        bool IsReady { get;}

        /// <summary>
        /// A value indicating the progress within current map script in percents. The value is 0 if no full script is running. 
        /// </summary>
        /// <value>The progress.</value>
        double Progress { get; }
        ObservableCollection<HeightData> Maps { get; }
        ObservableCollection<ScriptArg> Args { get; }

        void Start(string script, bool headerOnly = false, IEnumerable<uint> parametersOverride = null);
        void Abort();
    }
}
