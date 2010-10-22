using System.ComponentModel;
using System.Collections.Generic;
using System.Reflection;
using System;

namespace GeoGen.Studio.Utilities
{
    public static class BindingManager
    {
        static private BindingTable bindings = new BindingTable();

        public static void BindOneWay(INotifyPropertyChanged fromObject, string fromProperty, object toObject, string toProperty)
        {
            BindingManager.bindings.Add(fromObject, fromProperty, toObject, toProperty);

            fromObject.PropertyChanged -= BindingManager.OnBoundPropertyChanged;
            fromObject.PropertyChanged += BindingManager.OnBoundPropertyChanged;
        }

        public static void Bind(INotifyPropertyChanged object1, string property1, INotifyPropertyChanged object2, string property2)
        {
            BindingManager.BindOneWay(object1, property1, object2, property2);
            BindingManager.BindOneWay(object2, property2, object1, property1);
        }

        public static void UnbindOneWay(INotifyPropertyChanged fromObject, string fromProperty = null, object toObject = null, string toProperty = null)
        {
            BindingManager.bindings.Remove(fromObject, fromProperty, toObject, toProperty);

            if(!BindingManager.bindings.ObjectHasBindings(fromObject))
            {
                fromObject.PropertyChanged -= BindingManager.OnBoundPropertyChanged;
            }
        }

        public static void Unbind(INotifyPropertyChanged object1, string property1 = null, INotifyPropertyChanged object2 = null, string property2 = null)
        {
            BindingManager.UnbindOneWay(object1, property1, object2, property2);
            BindingManager.UnbindOneWay(object2, property2, object1, property1);
        }

        private static void OnBoundPropertyChanged(object sender, PropertyChangedEventArgs args)
        {
            PropertyInfo fromProperty = null;

            try{
                fromProperty = sender.GetType().GetProperty(args.PropertyName);

                if(!BindingManager.IsPropertyReadable(fromProperty))
                {
                    throw new Exception();
                }
            }
            catch
            {
                throw new InvalidBoundPropertyException("Bound property \"" + args.PropertyName + "\" doesn't exist or is not readable.");
            }
            
            Dictionary<object, string> foundBindings = BindingManager.bindings.GetBindingsByProperty(sender as INotifyPropertyChanged, args.PropertyName);

            foreach (KeyValuePair<object, string> binding in foundBindings)
            {
                PropertyInfo toProperty = null;

                try
                {
                    toProperty = binding.Key.GetType().GetProperty(binding.Value);

                    if (!BindingManager.IsPropertyWritable(toProperty, binding.Key.GetType()) || !BindingManager.IsPropertyReadable(toProperty))
                    {
                        throw new Exception();
                    }
                }
                catch
                {
                    throw new InvalidBoundPropertyException("Bound property \"" + binding.Value + "\" doesn't exist or is not readable or writable.");
                }

                try
                {
                    object fromValue = fromProperty.GetValue(sender, null);
                    object toValue = toProperty.GetValue(binding.Key, null);

                    if(!fromValue.Equals(toValue))
                    {
                        toProperty.SetValue(binding.Key, fromValue, null);
                    }
                }
                catch (Exception e)
                {
                    throw new BindingException("An exception was thrown while property value was being copied.", e);
                }
            }
        }

        private static bool IsPropertyReadable(PropertyInfo property)
        {
            return
                property != null &&
                property.CanRead
            ;
        }

        private static bool IsPropertyWritable(PropertyInfo property, Type type)
        {
            return
                property != null &&
                property.CanWrite /*&&
                property.PropertyType.IsAssignableFrom(type)*/
            ;
        }
    }
}
