using GeoGen.Studio.Utilities.Collections;
using GeoGen.Studio.Utilities.Persistence;

namespace GeoGen.Studio.PlugInLoader
{
    internal sealed class LoaderConfig
    {
        [Persistent(UseEmptyInstanceAsDefault = true)]
        public TypeCollection DisabledPlugIns { get; private set; }

        public LoaderConfig()
        {
            this.DisabledPlugIns = new TypeCollection();
        }
    }
}
