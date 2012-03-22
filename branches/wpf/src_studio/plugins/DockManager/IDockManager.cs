using System.Collections.ObjectModel;
using System.Windows;
using GeoGen.Studio.PlugInLoader;

namespace GeoGen.Studio.PlugIns
{
    public enum DockingLocation
    {
        Document,
        Floating,
        Left,
        LeftTop,
        LeftBottom,
        Right,
        RightTop,
        RightBottom,
        Top,
        TopLeft,
        TopRight,
        Bottom,
        BottomLeft,
        BottomRight
    }

    public interface IDockManager: IPlugInInterface
    {
        bool AddAsDocumentContent(object content, string title, bool focus = false);
        bool AddAsDockableContent(object content, string title, DockingLocation preferredLocation);
        bool AddAsFloatableWindow(object content, string title, Size size, bool dockable = true);
        void Activate(object content);
        object GetDockingState(object content);
        object GetDockingStateByLocation(DockingLocation location);
        bool IsContentActive(object content);
    }
}
