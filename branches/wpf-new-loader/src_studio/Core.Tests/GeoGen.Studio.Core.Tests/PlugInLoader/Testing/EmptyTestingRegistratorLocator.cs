namespace GeoGen.Studio.PlugInLoader.Testing
{
    using System;
    using System.Collections.Generic;
    using System.Linq;

    /// <summary>
    /// Registrator locator which always returns empty collection of registrators.
    /// </summary>
    public class EmptyTestingRegistratorLocator : IRegistratorLocator
    {
        public IEnumerable<IRegistrator> GetRegistrators(Type type)
        {
            return Enumerable.Empty<IRegistrator>();
        }
    }
}
