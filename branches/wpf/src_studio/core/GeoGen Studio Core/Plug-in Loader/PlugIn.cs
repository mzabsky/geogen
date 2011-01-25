using System;
using System.Collections.Generic;
using System.Reflection;

namespace GeoGen.Studio.PlugInLoader
{
    /// <summary>
    /// Holds all available information about a single plug-in.
    /// </summary>
    public sealed class PlugIn
    {
        /// <summary>
        /// User-friendly name of the plug-in.
        /// </summary>
        /// <value>The name.</value>
        public string Name {get; private set;}

        /// <summary>
        /// Short user-friendly description of the plug-in.
        /// </summary>
        /// <value>The name.</value>
        public string Description { get; private set;}

        /// <summary>
        /// <see cref="Assembly"/> in which this plug-in is defined.
        /// </summary>
        /// <value>The <see cref="Assembly"/>.</value>
        public Assembly Assembly {get; private set;}

        /// <summary>
        /// <see cref="Type"/> defining this plug-in.
        /// </summary>
        /// <value>The <see cref="Type"/>.</value>
        public Type Type {get; private set;}

        private readonly List<object> instances = new List<object>();
        /// <summary>
        /// List of instances started by the <see cref="Loader"/>.
        /// </summary>
        /// <value>The instances.</value>
        public IEnumerable<object> Instances
        {
            get
            {
                return this.instances.AsReadOnly();
            }
        }

        private readonly List<Registrator> registrators = new List<Registrator>();
        /// <summary>
        /// List of <see cref="Registrator">registrators</see> defined by this plug-in.
        /// </summary>
        /// <value>The <see cref="Registrator">registrators</see>.</value>
        public IEnumerable<Registrator> Registrators
        {
            get
            {
                return this.registrators.AsReadOnly();
            }
        }

        /// <summary>
        /// Indicates whether this plug-in will be loaded next time the application is started.
        /// </summary>
        /// <value>
        /// 	<c>true</c> if this plug-in is enabled; otherwise, <c>false</c>.
        /// </value>
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

        /// <summary>
        /// Gets a value indicating whether at least one instance of this plug-in is running.
        /// </summary>
        /// <value>
        /// 	<c>true</c> if at least one instance of this plug-in is running; otherwise, <c>false</c>.
        /// </value>
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
            this.Name = plugInAttribute.Name != "" ? plugInAttribute.Name : plugInType.Name;

            this.Description = plugInAttribute.Description;

            this.instances = new List<object>(instances);
            this.registrators = new List<Registrator>(registrators);
        }
    }
}
