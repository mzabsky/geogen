using System;

namespace GeoGen.Studio.Utilities.Configurability
{
    [AttributeUsage(AttributeTargets.Property, Inherited = true, AllowMultiple = false)]
    public class ConfigurableAttribute : Attribute
    {
        public object DefaultValue { get; private set; }

        public ConfigurableAttribute(){}

        public ConfigurableAttribute(object defaultValue)
        {
            this.DefaultValue = defaultValue;
        }
    }
}
