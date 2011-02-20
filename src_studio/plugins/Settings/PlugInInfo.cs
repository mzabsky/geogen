using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Reflection;
using GeoGen.Studio.PlugInLoader;
using GeoGen.Studio.Utilities;
using GeoGen.Studio.Utilities.Configurability;

namespace GeoGen.Studio.PlugIns
{
    public class PlugInInfo: ObservableObject, INotifyPropertyChanged
    {
        public PlugIn PlugIn { get; set; }
        public List<ConfigurablePropertyInfo> Properties {get; set;}
        public bool IsEnabled { get; set; }

        public PlugInInfo(PlugInLoader.PlugIn plugIn)
        {
            this.IsEnabled = Loader.IsPlugInEnabled(plugIn);
            this.PlugIn = plugIn;
            this.Properties = new List<ConfigurablePropertyInfo>();            

            // Not every plug-in is necessarily configurable
            if (!typeof(IConfigurable).IsAssignableFrom(this.PlugIn.Type))
            {
                return ;
            }

            // Build the list of configurable properties
            foreach (PropertyInfo property in plugIn.Type.GetProperties())
            {              
                ConfigurableAttribute configurableAttribute = Attribute.GetCustomAttribute(property, typeof(ConfigurableAttribute)) as ConfigurableAttribute;

                /* Skip non-readable non-readable non-configurable properties. */
                if (property.CanWrite && property.CanRead && configurableAttribute != null)
                {
                    ConfigurablePropertyInfo configurablePropertyInfo = new ConfigurablePropertyInfo();
                    configurablePropertyInfo.Name = property.Name;
                    configurablePropertyInfo.Property = property;
                    configurablePropertyInfo.Type = property.PropertyType;
                    configurablePropertyInfo.Description = "";

                    if(this.PlugIn.IsRunning)
                    {
                        // The plug-in is running - get the value from its first instance
                        configurablePropertyInfo.OriginalValue = property.GetValue(Enumerable.First(this.PlugIn.Instances), null);
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
    }
}
