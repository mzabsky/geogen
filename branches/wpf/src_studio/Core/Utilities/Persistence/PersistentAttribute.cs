using System;

namespace GeoGen.Studio.Utilities.Persistence
{
    /// <summary>
    /// Makes a property persistent (if the object instance is registered with <see cref="MainConfig"/>).
    /// </summary>
    [AttributeUsage(AttributeTargets.Property | AttributeTargets.Class, Inherited = true, AllowMultiple = false)]
    public sealed class PersistentAttribute : Attribute
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
        ///     <c>true</c> if a default instance should be used as default value; otherwise, <c>false</c>.
        /// </value>
        public bool UseEmptyInstanceAsDefault{get; set;}

        /// <summary>
        /// Allow visual configuration of this property.
        /// </summary>
        /// <value>
        ///     <c>true</c> this property should be displayed in visual configuration tools, <c>false</c>.
        /// </value>
        public bool EnableVisualConfiguration { get; set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="PersistentAttribute"/> class.
        /// </summary>
        public PersistentAttribute(){}

        /// <summary>
        /// Initializes a new instance of the <see cref="PersistentAttribute"/> class.
        /// </summary>
        /// <param name="defaultValue">The default value.</param>
        public PersistentAttribute(object defaultValue = null)
        {
            this.DefaultValue = defaultValue;
            this.EnableVisualConfiguration = true;
        }
    }
}
