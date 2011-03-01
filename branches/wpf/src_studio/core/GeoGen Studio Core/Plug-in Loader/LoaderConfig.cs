using GeoGen.Studio.Utilities.Collections;
using GeoGen.Studio.Utilities.Configurability;

namespace GeoGen.Studio.PlugInLoader
{
    internal sealed class LoaderConfig
    {
        [Configurable(UseEmptyInstanceAsDefault = true)]
        public TypeCollection DisabledPlugIns { get; private set; }

        public LoaderConfig()
        {
            this.DisabledPlugIns = new TypeCollection();
        }
    }
}
