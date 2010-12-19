using System;
using System.Collections.Generic;
using GeoGen.Studio.Utilities.Configurability;

namespace GeoGen.Studio.PlugInLoader
{
    internal sealed class LoaderConfig: IConfigurable
    {
        [Configurable]
        public List<Type> EnabledPlugIns {get; private set;}

        [Configurable]
        public List<Type> DisabledPlugIns { get; private set; }

        public LoaderConfig()
        {
            this.EnabledPlugIns = new List<Type>();
            this.DisabledPlugIns = new List<Type>();
        }
    }
}
