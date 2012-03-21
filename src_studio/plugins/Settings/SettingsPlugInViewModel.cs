namespace GeoGen.Studio.PlugIns
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Linq;

    using GeoGen.Studio.PlugInLoader;
    using GeoGen.Studio.Utilities;
    using GeoGen.Studio.Utilities.Persistence;

    /// <summary>
    /// Represents one plug-in type within the Settings dialog.
    /// </summary>
    public class SettingsPlugInViewModel : ObservableObject, INotifyPropertyChanged
    {        
        /// <summary>
        /// Initializes a new instance of the <see cref="SettingsPlugInViewModel"/> class.
        /// </summary>
        /// <param name="plugIn">The plug in.</param>
        public SettingsPlugInViewModel(PlugInLoader.PlugIn plugIn)
        {
            this.IsEnabled = Loader.IsPlugInEnabled(plugIn);
            this.PlugIn = plugIn;
            this.Properties = new List<SettingsPropertyViewModel>();
            
            this.IsEnabled = false;
            this.IsEnabled = true;

            this.PropertyChanged += delegate(object o, PropertyChangedEventArgs a)
                {
                    Console.WriteLine(a.PropertyName);
                };

            // Build the list of configurable properties
            this.ParseProperties(plugIn);
        }

        /// <summary>
        /// Gets or sets the plug-in object obtained from the loader.
        /// </summary>
        /// <value>
        /// The plug-in object.
        /// </value>
        public PlugIn PlugIn { get; set; }

        /// <summary>
        /// Gets or sets collection of configuation information related to individual properties.
        /// </summary>
        /// <value>
        /// The properties.
        /// </value>
        public List<SettingsPropertyViewModel> Properties { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether this plug-in is enabled is enabled.
        /// </summary>
        /// <value>
        ///     <c>true</c> if this instance is enabled; otherwise, <c>false</c>.
        /// </value>
        public bool IsEnabled { get; set; }

        /// <summary>
        /// Returns a <see cref="System.String"/> that represents this instance.
        /// </summary>
        /// <returns>
        /// A <see cref="System.String"/> that represents this instance.
        /// </returns>
        public override string ToString()
        {
            return this.PlugIn.Name;
        }

        /// <summary>
        /// Parses persistent properties of the plug-in into information, which can be displayed within the Settings dialog.
        /// </summary>
        /// <param name="plugIn">The plug in.</param>
        private void ParseProperties(PlugIn plugIn)
        {
            foreach (var property in plugIn.Type.GetProperties())
            {
                var configurableAttribute =
                    Attribute.GetCustomAttribute(property, typeof(PersistentAttribute)) as PersistentAttribute;

                // Skip non-readable non-readable non-configurable properties.
                if (property.CanWrite && property.CanRead && configurableAttribute != null
                    && configurableAttribute.EnableVisualConfiguration)
                {
                    var configurablePropertyInfo = new SettingsPropertyViewModel
                        { Name = property.Name, Property = property, Type = property.PropertyType, Description = string.Empty };

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
    }
}
