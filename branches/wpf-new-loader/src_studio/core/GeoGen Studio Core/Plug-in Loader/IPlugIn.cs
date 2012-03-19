namespace GeoGen.Studio.PlugInLoader
{
    /// <summary>
    /// Any type implementing this interface will be considered to be a plug-in.
    /// </summary>
    /// <remarks>
    /// The plug-in also needs to have at least one <see cref="Registrator"/> to be loaded by the <see cref="Loader"/>.
    /// </remarks>
    public interface IPlugIn
    {
    }
}
