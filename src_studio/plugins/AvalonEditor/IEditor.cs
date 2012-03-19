namespace GeoGen.Studio.PlugIns
{
	using System.ComponentModel;
	using GeoGen.Studio.PlugInLoader;
	using GeoGen.Studio.PlugIns.Interfaces;

	public interface IEditor: IPlugInInterface, INotifyPropertyChanged, IControl
	{
		string Text {get; set;}
	}
}
