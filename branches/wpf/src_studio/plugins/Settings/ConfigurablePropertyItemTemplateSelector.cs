using System.Windows;
using System.Windows.Controls;
using System;

namespace GeoGen.Studio.PlugIns
{
    internal class ConfigurablePropertyItemTemplateSelector : DataTemplateSelector
    {
        public DataTemplate Int { get; set; }
        public DataTemplate Float { get; set; }
        public DataTemplate Bool { get; set; }
        public DataTemplate String { get; set; }
        public DataTemplate UnknownType { get; set; }

        public override DataTemplate SelectTemplate(object item, DependencyObject container)
        {
            ConfigurablePropertyInfo info = item as ConfigurablePropertyInfo;           

            if (
                info.Value is int ||
                info.Value is uint ||
                info.Value is long ||
                info.Value is ulong ||
                info.Value is short ||
                info.Value is ushort ||
                info.Value is byte ||
                info.Value is float ||
                info.Value is double
            )
            {
                return Int;
            }
            else if (
                info.Value is float || 
                info.Value is double
            )
            {
                return Float;
            }
            else if (info.Value is bool)
            {
                return Bool;
            }
            else if (info.Value is string)
            {
                return String;
            }
            else
            {
                return UnknownType;
            }
            return base.SelectTemplate(item, container);
        }

        public ConfigurablePropertyItemTemplateSelector()
        {            
        }
    }

}
