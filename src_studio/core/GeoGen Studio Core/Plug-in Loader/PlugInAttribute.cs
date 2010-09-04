using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GeoGen_Studio
{
    public enum InstanceCount{
        One,
        OnePerRegistration
    };

    [AttributeUsage(AttributeTargets.Class, Inherited = true, AllowMultiple = false)]
    public class PlugInAttribute : Attribute
    {
        public InstanceCount InstanceCount { get; private set; }

        public PlugInAttribute(InstanceCount instanceCount = InstanceCount.One)
        {
            this.InstanceCount = instanceCount;
        }
    }
}
