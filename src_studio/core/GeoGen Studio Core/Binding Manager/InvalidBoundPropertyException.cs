using System;

namespace GeoGen_Studio
{
    public class InvalidBoundPropertyException: BindingException
    {
        public InvalidBoundPropertyException(string message): base(message) {}
    }
}
