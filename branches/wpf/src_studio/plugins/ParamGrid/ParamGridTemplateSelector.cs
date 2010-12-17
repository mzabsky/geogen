using System.Windows.Controls;
using System.Windows;
using GeoGen.Net;

namespace GeoGen.Studio.PlugIns
{
    public class ParamGridTemplateSelector : DataTemplateSelector
    {
        public DataTemplate BoolTemplate { get; set; }
        public DataTemplate IntTemplate { get; set; }
        public DataTemplate EnumTemplate { get; set; }

        public override DataTemplate SelectTemplate(object item, DependencyObject container)
        {
            if (item is ScriptArg)
            {
                ScriptArg arg = (ScriptArg)item;

                switch (arg.Type)
                {
                    case ScriptArgType.Bool: return BoolTemplate;
                    case ScriptArgType.Int: return IntTemplate;
                    case ScriptArgType.Enum: return EnumTemplate;
                }

                return null;
            }
            return base.SelectTemplate(item, container);
        }

        public ParamGridTemplateSelector()
        {
            this.BoolTemplate = null;
            this.IntTemplate = null;
            this.EnumTemplate = null;
        }
    }
}
