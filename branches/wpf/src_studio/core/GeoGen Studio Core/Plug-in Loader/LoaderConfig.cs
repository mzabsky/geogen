using System;
using System.Collections.Generic;
using GeoGen.Studio.Utilities.Configurability;

namespace GeoGen.Studio.PlugInLoader
{
    internal sealed class LoaderConfig: IConfigurable
    {
        [Configurable(UseEmptyInstanceAsDefault = true)]
        public List<Type> DisabledPlugIns { get; private set; }

        public LoaderConfig()
        {
            this.DisabledPlugIns = new List<Type>();
        }
    }
}
