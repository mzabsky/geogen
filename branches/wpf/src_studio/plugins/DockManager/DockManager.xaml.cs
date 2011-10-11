using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Collections.ObjectModel;
using GeoGen.Studio.PlugInLoader;

namespace GeoGen.Studio.PlugIns
{    
    [PlugIn(VisibleInList = false)]
    public partial class DockManager : UserControl, IPlugIn, IDockManager
    {
        Dictionary<object, object> registeredContents = new Dictionary<object, object>();        

        public DockManager()
        {
            this.InitializeComponent();
        }

        public void Register(IMainWindow mainWindow)
        {
            mainWindow.Content = this;
        }

        public bool AddAsDocumentContent(object content, string title, bool focus = false){
            if(registeredContents.ContainsKey(content))
            {
                dynamic existingContent = registeredContents[content];
                
                if (!existingContent.IsArrangeValid) existingContent.Show();
                existingContent.Activate();

                return false;
            }

            AvalonDock.DocumentContent avalonContent = new AvalonDock.DocumentContent();
            avalonContent.Content = content;
            avalonContent.Title = title;
            avalonContent.Show(this.dockManager);

            if(focus)
            {
                avalonContent.Activate();
            }

            registeredContents.Add(content, avalonContent);

            return true;
        }

        public bool AddAsDockableContent(object content, string title, bool showInMainArea = false)
        {
            if (registeredContents.ContainsKey(content))
            {
                dynamic existingContent = registeredContents[content];

                if (!existingContent.IsArrangeValid) existingContent.Show();
                existingContent.Activate();

                return false;
            }

            AvalonDock.DockableContent avalonContent = new AvalonDock.DockableContent();
            avalonContent.Content = content;
            avalonContent.Title = title;
            avalonContent.Show(this.dockManager);

            if(showInMainArea)
            {
                avalonContent.ShowAsDocument(this.dockManager);
            }
            else
            {
                avalonContent.Show();
            }

            registeredContents.Add(content, avalonContent);

            return true;
        }

        public bool AddAsFloatableWindow(object content, string title, Size size, bool dockable = true)
        {
            if (registeredContents.ContainsKey(content))
            {
                dynamic existingContent = registeredContents[content];

                if (!existingContent.IsArrangeValid) existingContent.ShowAsFloatingWindow();
                existingContent.Activate();

                return false;
            }

            AvalonDock.DockableContent avalonContent = new AvalonDock.DockableContent();
            avalonContent.Content = content;
            avalonContent.Title = title;
            avalonContent.Show(this.dockManager);

            if(size != null)
            {
                avalonContent.FloatingWindowSize = size;
            }

            avalonContent.ShowAsFloatingWindow(this.dockManager, dockable);

            registeredContents.Add(content, avalonContent);

            return true;
        }

        public void Activate(object content)
        {
            if (!registeredContents.ContainsKey(content))
            {
                throw new InvalidOperationException("Passed content is not registered with this dock manager.");
            }
            
            dynamic avalonContent = registeredContents[content];
            avalonContent.Activate();
        }
    
    }
}
