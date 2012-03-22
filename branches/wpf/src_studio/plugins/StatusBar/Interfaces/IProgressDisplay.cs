namespace GeoGen.Studio.PlugIns.Interfaces
{
    using GeoGen.Studio.PlugInLoader;

    public interface IProgressDisplay : IPlugInInterface
    {
        void SetProgress(object key, double? progress);
    }
}
