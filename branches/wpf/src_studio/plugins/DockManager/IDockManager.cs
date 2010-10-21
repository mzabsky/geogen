using System.Collections.ObjectModel;
using System.Windows;

namespace GeoGen_Studio
{
    public interface IDockManager: IPlugInInterface
    {
        bool AddAsDocumentContent(object content, string title, bool focus = false);
        bool AddAsDockableContent(object content, string title, bool showInMainArea = false);
        bool AddAsFloatableWindow(object content, string title, Size size, bool dockable = true);
    }
}
