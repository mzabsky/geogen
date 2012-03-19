namespace GeoGen.Studio.PlugIns.Interfaces
{
	using GeoGen.Studio.PlugInLoader;
	using GeoGen.Studio.Utilities.Context;

	public interface IApplicationStatusDisplay: IPlugInInterface
	{
		void RegisterApplicationStatusContext(Context context);
	}
}
