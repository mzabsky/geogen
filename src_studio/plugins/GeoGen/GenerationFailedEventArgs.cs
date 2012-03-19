using System;

namespace GeoGen.Studio.PlugIns
{
    public class GenerationFailedEventArgs: EventArgs
    {
        public string Message { get; protected set; }
        public bool IsHeaderLoaded {get; protected set;}

        public GenerationFailedEventArgs(string message, bool IsHeaderLoaded)
        {
            this.Message = message;
            this.IsHeaderLoaded = IsHeaderLoaded;
        }
    }
}
