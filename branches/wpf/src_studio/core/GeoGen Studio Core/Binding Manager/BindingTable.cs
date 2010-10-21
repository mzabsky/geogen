using System.Collections.Generic;
using System.ComponentModel;

namespace GeoGen_Studio
{
    internal class BindingTable{
        Dictionary<INotifyPropertyChanged, Dictionary<string, Dictionary<object, List<string>>>> bindings = new Dictionary<INotifyPropertyChanged, Dictionary<string, Dictionary<object, List<string>>>>();

        public void Add(INotifyPropertyChanged fromObject, string fromProperty, object toObject, string toProperty)
        {
            /* First dimension - fromObject. */
            if(!this.bindings.ContainsKey(fromObject)){
                this.bindings.Add(fromObject, new Dictionary<string, Dictionary<object, List<string>>>());
            }

            Dictionary<string, Dictionary<object, List<string>>> bindingsByFromProperty = this.bindings[fromObject];

            /* Second dimension - fromProperty. */
            if (!bindingsByFromProperty.ContainsKey(fromProperty))
            {
                bindingsByFromProperty.Add(fromProperty, new Dictionary<object, List<string>>());
            }

            Dictionary<object, List<string>> bindingsByToObject = bindingsByFromProperty[fromProperty];

            /* Third dimension - toObject. */
            if (!bindingsByToObject.ContainsKey(toObject))
            {
                bindingsByToObject.Add(toObject, new List<string>());
            }

            List<string> bindingsByToProperty = bindingsByToObject[toObject];

            /* Fourth dimension - toProperty. */
            if (!bindingsByToProperty.Contains(toProperty))
            {
                bindingsByToProperty.Add(toProperty);
            }
        }

        public void Clear()
        {
            this.bindings.Clear();
        }

        public void Remove(INotifyPropertyChanged fromObject, string fromProperty = null, object toObject = null, string toProperty = null)
        {
            /* First dimension - fromObject. */
            if (!this.bindings.ContainsKey(fromObject))
            {
                return;
            }

            Dictionary<string, Dictionary<object, List<string>>> bindingsByFromProperty = this.bindings[fromObject];

            /* Second dimension - fromProperty. */
            if(fromProperty == null)
            {
                bindingsByFromProperty.Clear();
            }
            else 
            {
                if (!bindingsByFromProperty.ContainsKey(fromProperty))
                {
                    return;
                }
                else
                {
                    Dictionary<object, List<string>> bindingsByToObject = bindingsByFromProperty[fromProperty];

                    /* Third dimension - toObject. */
                    if(toObject == null)
                    {
                        bindingsByToObject.Clear();
                    }
                    else{
                        if(!bindingsByToObject.ContainsKey(toObject)){
                            return;
                        }
                        else
                        {
                            List<string> bindingsByToProperty = bindingsByToObject[toObject];

                            /* Fourth dimension - toProperty. */
                            if(toProperty == null)
                            {
                                bindingsByToProperty.Clear();
                            }
                            else
                            {
                                bindingsByToProperty.Remove(toProperty);
                            }

                            if(bindingsByToProperty.Count == 0)
                            {
                                bindingsByToObject.Remove(toObject);
                            }
                        }
                    }

                    if (bindingsByToObject.Count == 0)
                    {
                        bindingsByFromProperty.Remove(fromProperty);
                    }
                }
            }

            if (bindingsByFromProperty.Count == 0)
            {
                bindings.Remove(fromObject);
            }
        }

        public Dictionary<object, string> GetBindingsByProperty(INotifyPropertyChanged fromObject, string fromProperty)
        {
            if(!this.bindings.ContainsKey(fromObject))
            {
                return new Dictionary<object, string>();
            }
            
            Dictionary<string, Dictionary<object, List<string>>> bindingsByFromProperty = this.bindings[fromObject];

            if(!bindingsByFromProperty.ContainsKey(fromProperty))
            {
                return new Dictionary<object, string>();
            }

            /* Compress the remaining two dimensions into one dimension, so the returned list can be easily foreached. */
            Dictionary<object, string> foundBindings = new Dictionary<object, string>();
            foreach (object toObject in bindingsByFromProperty[fromProperty].Keys)
            {
                foreach(string toProperty in bindingsByFromProperty[fromProperty][toObject]){
                    foundBindings.Add(toObject, toProperty);
                }
            }

            return foundBindings;
        }

        public bool ObjectHasBindings(INotifyPropertyChanged fromObject)
        {
            if (!this.bindings.ContainsKey(fromObject))
            {
                return true;
            }

            return false;
        }
    }
}
