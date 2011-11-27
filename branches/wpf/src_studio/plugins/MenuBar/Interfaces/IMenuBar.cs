namespace GeoGen.Studio.PlugIns.Interfaces
{
	using System.Windows.Controls;
	using GeoGen.Studio.PlugInLoader;
	using GeoGen.Studio.PlugIns.MenuBars;

	public interface IMenuBar: IPlugInInterface, IControl
	{
		void AddMenu(MenuItem menu);
		void AddMenu(MenuEntry menu);
	}
}
