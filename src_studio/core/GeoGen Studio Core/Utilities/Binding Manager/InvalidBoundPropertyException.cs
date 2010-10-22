using System;

namespace GeoGen.Studio.Utilities
{
    public class InvalidBoundPropertyException: BindingException
    {
        public InvalidBoundPropertyException(string message): base(message) {}
    }
}
