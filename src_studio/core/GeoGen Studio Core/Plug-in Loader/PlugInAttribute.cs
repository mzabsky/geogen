using System;

namespace GeoGen.Studio.PlugInLoader
{
    /// <summary>
    /// Specifies whether creation of multiple plug-in instances is allowed in case plug-in's <see cref="Registrator"/>s is called with multiple parameter combinations.
    /// </summary>
    public enum InstanceCount{
        /// <summary>
        /// Create one plug-in instance only and call the <see cref="Registrator"/>s on it.
        /// </summary>
        One,
        /// <summary>
        /// Create a new plug-in instance for each <see cref="Registrator"/> call.
        /// </summary>
        OnePerRegistration
    };

    /// <summary>
    /// Attribute specifying how <see cref="Loader"/> handles the plug-in.
    /// </summary>
    [AttributeUsage(AttributeTargets.Class, Inherited = true, AllowMultiple = false)]
    public sealed class PlugInAttribute : Attribute
    {
        /// <summary>
        /// User-friendly name of the plug-in.
        /// </summary>
        /// <value>The name.</value>
        public string Name { get; set; }

        /// <summary>
        /// Short description of the plug-in.
        /// </summary>
        /// <value>The description.</value>
        public string Description { get; set; }
        
        /// <summary>
        /// Specifies whether creation of multiple plug-in instances is allowed in case plug-in's <see cref="Registrator"/>s is called with multiple parameter combinations.
        /// </summary>
        public InstanceCount InstanceCount { get; set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="PlugInAttribute"/> class.
        /// </summary>        
        /// <param name="name">User-friendly name of the plug-in</param>
        /// <param name="description">Short user-friendly description of the plug-in</param>        
        /// <param name="instanceCount">Specifies whether creation of multiple plug-in instances is allowed in case plug-in's <see cref="Registrator"/>s is called with multiple parameter combinations.</param>
        public PlugInAttribute(string name = "", string description = "", InstanceCount instanceCount = InstanceCount.One)
        {
            this.Name = name;
            this.Description = description;
            this.InstanceCount = instanceCount;
        }
    }
}
