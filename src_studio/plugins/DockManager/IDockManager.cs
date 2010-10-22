using System.Collections.ObjectModel;
using System.Windows;
using GeoGen.Studio.PlugInLoader;

namespace GeoGen.Studio.PlugIns
{
    public interface IDockManager: IPlugInInterface
    {
        bool AddAsDocumentContent(object content, string title, bool focus = false);
        bool AddAsDockableContent(object content, string title, bool showInMainArea = false);
        bool AddAsFloatableWindow(object content, string title, Size size, bool dockable = true);
    }
}
