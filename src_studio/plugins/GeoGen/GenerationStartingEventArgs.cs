using System;

namespace GeoGen.Studio.PlugIns
{
    public class GenerationStartingEventArgs: GenerationStartedEventArgs
    {        
        public bool Continue {get; set;}

        public GenerationStartingEventArgs() {}

        public GenerationStartingEventArgs(string script, bool headerOnly)
        {
            this.Script = script;
            this.HeaderOnly = false;
            this.Continue = true;
        }
    }
}
