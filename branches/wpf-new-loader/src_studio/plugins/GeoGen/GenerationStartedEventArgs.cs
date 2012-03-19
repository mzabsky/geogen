using System;

namespace GeoGen.Studio.PlugIns
{
    public class GenerationStartedEventArgs: EventArgs
    {
        public string Script { get; protected set; }
        public bool HeaderOnly { get; set; }

        public GenerationStartedEventArgs() {}

        public GenerationStartedEventArgs(string script, bool headerOnly)
        {
            this.Script = script;
            this.HeaderOnly = headerOnly;
        }
    }
}
