namespace GeoGen.Studio.PlugInLoader.Testing
{
    using System;

    /// <summary>
    /// Simple testing plug-in with a single registrator and one non-plug-in interface.
    /// </summary>
    public class SimpleTestingPlugIn : ITestingPlugInInterface1, IEmptyTestingPlugInInterface, ICloneable
    {
        /// <summary>
        /// Gets or sets a value indicating whether the registrator was executed.
        /// </summary>
        /// <value>
        ///   <c>true</c> if [registrator was executed]; otherwise, <c>false</c>.
        /// </value>
        public bool RegistratorExecuted { get; set; }

        /// <summary>
        /// Testing registrator.
        /// </summary>
        /// <param name="o">The o.</param>
        public void Register(ITestingPlugInInterface2 o)
        {
            this.RegistratorExecuted = true;
        }
        
        public void Bar()
        {
            throw new NotImplementedException();
        }

        public object Clone()
        {
            throw new NotImplementedException();
        }
    }
}
