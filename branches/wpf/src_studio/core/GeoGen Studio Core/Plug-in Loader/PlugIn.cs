using System;
using System.Collections.Generic;
using System.Reflection;

namespace GeoGen.Studio.PlugInLoader
{
    public sealed class PlugIn
    {
        public string Name {get; private set;}
        public string Description { get; private set;}
        public Assembly Assembly {get; private set;}
        public Type Type {get; private set;}

        private List<object> instances = new List<object>();
        public IEnumerable<object> Instances
        {
            get
            {
                return this.instances.AsReadOnly();
            }
        }

        private List<Registrator> registrators = new List<Registrator>();
        public IEnumerable<Registrator> Registrators
        {
            get
            {
                return this.registrators.AsReadOnly();
            }
        }

        public bool IsEnabled {
            get
            {
                return Loader.IsPlugInEnabled(this);
            }
            set
            {
                if(value)
                {
                    Loader.EnablePlugIn(this);
                }
                else
                {
                    Loader.DisablePlugIn(this);
                }
            }
        }

        public bool IsRunning {
            get
            {
                return this.instances.Count > 0;
            }
        }

        internal PlugIn(Type plugInType, IEnumerable<object> instances, IEnumerable<Registrator> registrators)
        {
            this.Type = plugInType;
            this.Assembly = plugInType.Assembly;

            PlugInAttribute plugInAttribute = Attribute.GetCustomAttribute(plugInType, typeof(PlugInAttribute)) as PlugInAttribute ?? new PlugInAttribute();

            // Use type name if the attribute didn't provide a nice name
            if(plugInAttribute.Name != "")
            {
                this.Name = plugInAttribute.Name;
            }
            else
            {
                this.Name = plugInType.Name;
            }

            this.Description = plugInAttribute.Description;

            this.instances = new List<object>(instances);
            this.registrators = new List<Registrator>(registrators);
        }
    }
}
