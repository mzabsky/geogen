using System;

namespace GeoGen.Studio.Utilities
{
    public class BindingException: Exception
    {
        public BindingException(string message): base(message) {}
        public BindingException(string message, Exception innerException) : base(message, innerException) { }
    }
}
