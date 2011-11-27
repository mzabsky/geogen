namespace GeoGen.Studio.PlugIns.Interfaces
{
	using GeoGen.Studio.PlugInLoader;
	using GeoGen.Studio.PlugIns.StatusBars;

	public interface IStatusBar: IPlugInInterface
	{
		void AddItem(StatusBarEntry item);
	}
}
