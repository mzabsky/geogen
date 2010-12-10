using System;

namespace GeoGen.Studio.Utilities.Binding
{
    /// <summary>
    /// Represents an exception thrown when either of bound properties is accessible or the properties' type doesn't match.
    /// </summary>
    public class InvalidBoundPropertyException: BindingException
    {
        public InvalidBoundPropertyException(string message): base(message) {}
    }
}
