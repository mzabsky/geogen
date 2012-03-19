namespace GeoGen.Studio.PlugInLoader
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Reflection;

    /// <summary>
    /// Holds all available information about a single plug-in.
    /// </summary>
    public sealed class PlugIn
    {
        private readonly List<object> instances = new List<object>();

        private readonly List<IRegistrator> registrators = new List<IRegistrator>();

        /// <summary>
        /// Initializes a new instance of the <see cref="PlugIn"/> class from a <see cref="Type"/>.
        /// </summary>
        /// <param name="plugInType">Type of the plug in.</param>
        /// <param name="registratorLocator">The registrator locator.</param>
        public PlugIn(Type plugInType, IRegistratorLocator registratorLocator)
        {
            this.Type = plugInType;
            this.Assembly = plugInType.Assembly;

            PlugInAttribute plugInAttribute = Attribute.GetCustomAttribute(plugInType, typeof(PlugInAttribute)) as PlugInAttribute ?? new PlugInAttribute();

            // Use type name if the attribute didn't provide a nice name
            this.Name = plugInAttribute.Name.Length > 0 ? plugInAttribute.Name : plugInType.Name;
            this.Description = plugInAttribute.Description;
            this.VisibleInList = plugInAttribute.VisibleInList;

            this.instances = new List<object>(this.instances);
            this.registrators = registratorLocator.GetRegistrators(plugInType).ToList();
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="PlugIn"/> class (for testing purposes only).
        /// </summary>
       internal PlugIn()
       {
       }

        /// <summary>
        /// Gets <see cref="Assembly"/> in which this plug-in is defined.
        /// </summary>
        /// <value>The <see cref="Assembly"/>.</value>
        public Assembly Assembly { get; internal set; }

        /// <summary>
        /// Gets short user-friendly description of the plug-in.
        /// </summary>
        /// <value>The name.</value>
        public string Description { get; internal set; }

        /// <summary>
        /// Gets a collection of instances started by the <see cref="OldLoader"/>.
        /// </summary>
        /// <value>The instances.</value>
        public IEnumerable<object> Instances
        {
            get
            {
                return this.instances.AsReadOnly();
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether this plug-in will be loaded next time the application is started.
        /// </summary>
        /// <value>
        /// 	<c>true</c> if this plug-in is enabled; otherwise, <c>false</c>.
        /// </value>
        public bool IsEnabled
        {
            get
            {
                return true;
                //return OldLoader.IsPlugInEnabled(this);
            }
            set
            {
                if (value)
                {
                    //OldLoader.EnablePlugIn(this);
                }
                else
                {
                    //OldLoader.DisablePlugIn(this);
                }
            }
        }

        /// <summary>
        /// Gets a value indicating whether at least one instance of this plug-in is running.
        /// </summary>
        /// <value>
        /// 	<c>true</c> if at least one instance of this plug-in is running; otherwise, <c>false</c>.
        /// </value>
        public bool IsRunning
        {
            get
            {
                return this.instances.Count > 0;
            }
        }

        /// <summary>
        /// Gets <see cref="Loader"/> owning this plug-in.
        /// </summary>
        public Loader Loader { get; internal set; }

        /// <summary>
        /// Gets user-friendly name of the plug-in.
        /// </summary>
        /// <value>The name.</value>
        public string Name { get; internal set; }

        /// <summary>
        /// Gets collection of <see cref="Registrator">registrators</see> defined by this plug-in.
        /// </summary>
        /// <value>The <see cref="Registrator">registrators</see>.</value>
        public IEnumerable<IRegistrator> Registrators
        {
            get
            {
                return this.registrators.AsEnumerable();
            }
        }

        /// <summary>
        /// Gets <see cref="Type"/> defining this plug-in.
        /// </summary>
        /// <value>The <see cref="Type"/>.</value>
        public Type Type { get; internal set; }

        /// <summary>
        /// Gets a value indicating whether this plug-in is visible in any GUI list.
        /// </summary>
        /// <value>
        ///   <c>true</c> if the plug-in is visible in GUI lists; otherwise, <c>false</c>.
        /// </value>
        public bool VisibleInList { get; internal set; }

        /// <summary>
        /// Creates instance of this plug-in.
        /// </summary>
        /// <returns>The instance.</returns>
        public object CreateInstance()
        {
            return Activator.CreateInstance(this.Type);
        }
    }
}
