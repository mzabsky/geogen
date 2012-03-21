namespace GeoGen.Studio.PlugIns
{
    using System;
    using System.ComponentModel;
    using System.Reflection;

    using GeoGen.Studio.Utilities;

    /// <summary>
    /// Represents one plug-in persistent property within the Settings dialog.
    /// </summary>
    public class SettingsPropertyViewModel : ObservableObject, INotifyPropertyChanged
    {
        /// <summary>
        /// Gets or sets the name of the property.
        /// </summary>
        /// <value>
        /// The name.
        /// </value>
        public string Name { get; set; }

        /// <summary>
        /// Gets or sets actual reflection representation of the property.
        /// </summary>
        /// <value>
        /// The property.
        /// </value>
        public PropertyInfo Property { get; set; }

        /// <summary>
        /// Gets or sets the type of the plug-in.
        /// </summary>
        public Type Type { get; set; }

        /// <summary>
        /// Gets or sets the original value of the property (before it was modified within the Settings dialog).
        /// </summary>
        /// <value>
        /// The original value.
        /// </value>
        public object OriginalValue { get; set; }

        /// <summary>
        /// Gets or sets the value of the property.
        /// </summary>
        /// <value>
        /// The value.
        /// </value>
        public object Value { get; set; }

        /// <summary>
        /// Gets or sets the description of the property.
        /// </summary>
        /// <value>
        /// The description.
        /// </value>
        public object Description { get; set; }

        /// <summary>
        /// Returns a <see cref="System.String"/> that represents this instance.
        /// </summary>
        /// <returns>
        /// A <see cref="System.String"/> that represents this instance.
        /// </returns>
        public override string ToString()
        {
            return this.Name;
        }
    }    
}
