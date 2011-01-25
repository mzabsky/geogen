using System.ComponentModel;
using System.Collections.Generic;
using System.Reflection;
using System;

namespace GeoGen.Studio.Utilities.Binding
{
    /// <summary>
    /// Provides easy way to synchronize two properties of two object properties. Neither of these properties must be <see cref="System.Windows.DependencyProperty"/>, but the source property (both properties in case of two-way binding) must implement the <see cref="INotifyPropertyChanged"/> interface.
    /// </summary>
    /// <remarks>
    /// Bindings created with this class must always be manually <see cref="BindingManager.Unbind">unbound</see> or memory leaks (zombie objects) may occur.
    /// </remarks>
    public static class BindingManager
    {
        static private readonly BindingTable bindings = new BindingTable();

        /// <summary>
        /// Creates an one-way binding between two properties. The binding has to be manually <see cref="BindingManager.UnbindOneWay">unbound</see> before the bound objects can be propertly released.
        /// </summary>        
        /// <param name="fromObject">Source object.</param>
        /// <param name="fromProperty">Source property. This property must have public getter.</param>
        /// <param name="toObject">Target object.</param>
        /// <param name="toProperty">Target property. This property must have public getter and setter.</param>
        public static void BindOneWay(INotifyPropertyChanged fromObject, string fromProperty, object toObject, string toProperty)
        {
            BindingManager.bindings.Add(fromObject, fromProperty, toObject, toProperty);

            fromObject.PropertyChanged -= BindingManager.OnBoundPropertyChanged;
            fromObject.PropertyChanged += BindingManager.OnBoundPropertyChanged;
        }

        /// <summary>
        /// Creates a two-way binding between two properties. The binding has to be manually <see cref="BindingManager.Unbind">unbound</see> before the bound objects can be propertly released.
        /// </summary>
        /// <remarks>
        /// Both properties must have public getter and setter.
        /// </remarks>
        /// <param name="object1">Object 1.</param>
        /// <param name="property1">Property 1.</param>
        /// <param name="object2">Object 2.</param>
        /// <param name="property2">Property 2.</param>
        public static void Bind(INotifyPropertyChanged object1, string property1, INotifyPropertyChanged object2, string property2)
        {
            BindingManager.BindOneWay(object1, property1, object2, property2);
            BindingManager.BindOneWay(object2, property2, object1, property1);
        }

        /// <summary>
        /// Removes an one-way binding between two properties (if it exists).
        /// </summary>
        /// <remarks>
        /// This method can be used to remove only one direction of a two-way binding created with <see cref="BindingManager.Bind"/>.
        /// </remarks>
        /// <param name="fromObject">Source object.</param>
        /// <param name="fromProperty">Source property.</param>
        /// <param name="toObject">Target object.</param>
        /// <param name="toProperty">Target property.</param>
        public static void UnbindOneWay(INotifyPropertyChanged fromObject, string fromProperty = null, object toObject = null, string toProperty = null)
        {
            BindingManager.bindings.Remove(fromObject, fromProperty, toObject, toProperty);

            if(!BindingManager.bindings.ObjectHasBindings(fromObject))
            {
                fromObject.PropertyChanged -= BindingManager.OnBoundPropertyChanged;
            }
        }

        /// <summary>
        /// Removes both one-way and two-way bindings between two properties (if any exist).
        /// </summary>
        /// <param name="object1">Object 1.</param>
        /// <param name="property1">Property 1.</param>
        /// <param name="object2">Object 2.</param>
        /// <param name="property2">Property 2.</param>
        public static void Unbind(INotifyPropertyChanged object1, string property1 = null, INotifyPropertyChanged object2 = null, string property2 = null)
        {
            BindingManager.UnbindOneWay(object1, property1, object2, property2);
            BindingManager.UnbindOneWay(object2, property2, object1, property1);
        }

        private static void OnBoundPropertyChanged(object sender, PropertyChangedEventArgs args)
        {
            PropertyInfo fromProperty;

            try{
                fromProperty = sender.GetType().GetProperty(args.PropertyName);



                if(!BindingManager.IsPropertyReadable(fromProperty))
                {
                    return;
                    //throw new Exception();
                }
            }
            catch
            {
                throw new InvalidBoundPropertyException("Bound property \"" + args.PropertyName + "\" doesn't exist or is not readable.");
            }
            
            Dictionary<object, string> foundBindings = BindingManager.bindings.GetBindingsByProperty(sender as INotifyPropertyChanged, args.PropertyName);

            foreach (KeyValuePair<object, string> binding in foundBindings)
            {
                PropertyInfo toProperty;

                try
                {
                    toProperty = binding.Key.GetType().GetProperty(binding.Value);

                    if (!BindingManager.IsPropertyWritable(toProperty) || !BindingManager.IsPropertyReadable(toProperty))
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

        private static bool IsPropertyWritable(PropertyInfo property)
        {
            return
                property != null &&
                property.CanWrite /*&&
                property.PropertyType.IsAssignableFrom(type)*/
            ;
        }
    }
}
