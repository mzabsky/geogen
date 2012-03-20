namespace GeoGen.Studio.PlugIns.Interfaces
{
	using System.Windows;
	using GeoGen.Studio.PlugInLoader;

	public interface IWindow : IPlugInInterface
	{
		Window Window {get;}
	}
}
