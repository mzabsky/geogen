using System;

namespace GeoGen.Studio.Utilities.Configurability
{
    [AttributeUsage(AttributeTargets.Property, Inherited = true, AllowMultiple = false)]
    public class ConfigurableAttribute : Attribute
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

        public ConfigurableAttribute(){}

        public ConfigurableAttribute(object defaultValue)
        {
            this.DefaultValue = defaultValue;
        }
    }
}
