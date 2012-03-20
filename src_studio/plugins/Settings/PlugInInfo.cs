namespace GeoGen.Studio.PlugIns
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Linq;

    using GeoGen.Studio.PlugInLoader;
    using GeoGen.Studio.Utilities;
    using GeoGen.Studio.Utilities.Persistence;

    public class PlugInInfo : ObservableObject, INotifyPropertyChanged
    {
        public PlugInInfo(PlugInLoader.PlugIn plugIn)
        {
            this.IsEnabled = Loader.IsPlugInEnabled(plugIn);
            this.PlugIn = plugIn;
            this.Properties = new List<ConfigurablePropertyInfo>();
            
            this.IsEnabled = false;
            this.IsEnabled = true;

            // Build the list of configurable properties
            foreach (var property in plugIn.Type.GetProperties())
            {              
                var configurableAttribute = Attribute.GetCustomAttribute(property, typeof(PersistentAttribute)) as PersistentAttribute;

                // Skip non-readable non-readable non-configurable properties.
                if (
                    property.CanWrite && 
                    property.CanRead && 
                    configurableAttribute != null && 
                    configurableAttribute.EnableVisualConfiguration)
                {
                    var configurablePropertyInfo = new ConfigurablePropertyInfo
                    {
                        Name = property.Name, 
                        Property = property, 
                        Type = property.PropertyType, 
                        Description = string.Empty
                    };

                    if (this.PlugIn.IsRunning)
                    {
                        // The plug-in is running - get the value from its first instance
                        configurablePropertyInfo.OriginalValue = property.GetValue(this.PlugIn.Instances.First(), null);
                    }
                    else
                    {
                        // The plug-in is not running - get the value from the config file
                        configurablePropertyInfo.OriginalValue = MainConfig.GetPropertyValue(property);
                    }

                    configurablePropertyInfo.Value = configurablePropertyInfo.OriginalValue;
                    this.Properties.Add(configurablePropertyInfo);
                }
            }
        }

        public PlugIn PlugIn { get; set; }

        public List<ConfigurablePropertyInfo> Properties { get; set; }

        public bool IsEnabled { get; set; }

        public override string ToString()
        {
            return this.PlugIn.Name;
        }
    }
}
