using System;
using System.ComponentModel;
using System.Reflection;
using GeoGen.Studio.Utilities;

namespace GeoGen.Studio.PlugIns
{
    public class ConfigurablePropertyInfo: ObservableObject, INotifyPropertyChanged
    {
        public string Name {get;set;}
        public PropertyInfo Property {get; set;}
        public Type Type {get; set;}
        public object OriginalValue {get; set;}
        public object Value { get; set; }
        public object Description { get; set; }

        public override string ToString()
        {
            return this.Name;
        }
    }    
}
