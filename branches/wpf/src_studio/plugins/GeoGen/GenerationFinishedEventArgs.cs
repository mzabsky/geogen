using System;

namespace GeoGen.Studio.PlugIns
{
    public class GenerationFinishedEventArgs: EventArgs
    {
        public TimeSpan TimeSpan { get; protected set; }
        
        public GenerationFinishedEventArgs(TimeSpan timeSpan)
        {
            this.TimeSpan = timeSpan;
        }
    }
}
