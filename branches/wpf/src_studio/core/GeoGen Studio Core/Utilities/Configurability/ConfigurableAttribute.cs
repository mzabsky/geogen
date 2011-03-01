using System;

namespace GeoGen.Studio.Utilities.Configurability
{
    /// <summary>
    /// Makes a property persistent (if the object instance is registered with <see cref="MainConfig"/>).
    /// </summary>
    [AttributeUsage(AttributeTargets.Property, Inherited = true, AllowMultiple = false)]
    public sealed class ConfigurableAttribute : Attribute
    {
        /// <summary>
        /// The value used if the entry in config doesn't exist.
        /// </summary>
        /// <value>The default value.</value>
        public object DefaultValue { get; set; }

        /// <summary>
        /// Create an instance using the type's parameter-less constructor if the entry in config doesn't exist.
        /// </summary>
        /// <value>
        /// 	<c>true</c> if a default instance should be used as default value; otherwise, <c>false</c>.
        /// </value>
        public bool UseEmptyInstanceAsDefault{get; set;}

        /// <summary>
        /// Initializes a new instance of the <see cref="ConfigurableAttribute"/> class.
        /// </summary>
        public ConfigurableAttribute(){}

        /// <summary>
        /// Initializes a new instance of the <see cref="ConfigurableAttribute"/> class.
        /// </summary>
        /// <param name="defaultValue">The default value.</param>
        public ConfigurableAttribute(object defaultValue)
        {
            this.DefaultValue = defaultValue;
        }
    }
}
