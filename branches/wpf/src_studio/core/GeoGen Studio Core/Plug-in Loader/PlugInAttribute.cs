using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

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
    /// Attribute specifying how <see cref="PlugInLoader"/> handles the plug-in. */
    /// </summary>
    [AttributeUsage(AttributeTargets.Class, Inherited = true, AllowMultiple = false)]
    public class PlugInAttribute : Attribute
    {
        /// <summary>
        /// Specifies whether creation of multiple plug-in instances is allowed in case plug-in's <see cref="Registrator"/>s is called with multiple parameter combinations.
        /// </summary>
        public InstanceCount InstanceCount { get; private set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="PlugInAttribute"/> class.
        /// </summary>
        /// <param name="instanceCount">Specifies whether creation of multiple plug-in instances is allowed in case plug-in's <see cref="Registrator"/>s is called with multiple parameter combinations.</param>
        public PlugInAttribute(InstanceCount instanceCount = InstanceCount.One)
        {
            this.InstanceCount = instanceCount;
        }
    }
}
