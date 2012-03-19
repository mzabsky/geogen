using System;

namespace GeoGen.Studio.PlugInLoader
{
    /// <summary>
    /// Indicates that a <see cref="Registrator"/> is optional and will not throw error if not loaded because of missing dependencies.
    /// </summary>
    [AttributeUsage(AttributeTargets.Method, Inherited = true, AllowMultiple = false)]
    public sealed class OptionalRegistratorAttribute : Attribute
    {
    }
}
