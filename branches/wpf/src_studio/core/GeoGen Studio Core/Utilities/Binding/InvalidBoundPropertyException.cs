using System;

namespace GeoGen.Studio.Utilities.Binding
{
    public class InvalidBoundPropertyException: BindingException
    {
        public InvalidBoundPropertyException(string message): base(message) {}
    }
}
