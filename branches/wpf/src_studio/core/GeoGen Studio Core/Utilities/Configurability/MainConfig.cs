using System;
using System.Collections.Generic;
using System.Reflection;

namespace GeoGen.Studio.Utilities.Configurability
{
    public static class MainConfig
    {
        private static bool isInitialized = false;
        private static PropertyStore propertyStore = null;
        private static List<IConfigurable> registeredConfigurables = new List<IConfigurable>();

        private static void Initialize()
        {
            /* Load the configuration  data from the config.xml file. */
            System.Xml.Serialization.XmlSerializer xs = new System.Xml.Serialization.XmlSerializer(typeof(PropertyStore));

            System.IO.StreamReader reader = null;
            try
            {
                reader = System.IO.File.OpenText("config.xml");

                MainConfig.propertyStore = xs.Deserialize(reader) as PropertyStore;
            }
            catch
            {
                MainConfig.propertyStore = new PropertyStore();
            }
            finally
            {
                try
                {
                    reader.Close();
                }
                catch { };
            }

            /* Save the config.xml when the application is terminating. */
            GeoGen.Studio.App.Current.Exit += delegate(object o, System.Windows.ExitEventArgs args)
            {
                MainConfig.SaveConfigurations();
            };

            /* The class is now ready to use. */
            MainConfig.isInitialized = true;
        }

        private static void SaveConfigurations()
        {
            foreach(IConfigurable configurable in MainConfig.registeredConfigurables)
            {
                MainConfig.SaveConfiguration(configurable);
            }

            System.Xml.Serialization.XmlSerializer xs = new System.Xml.Serialization.XmlSerializer(typeof(PropertyStore));

            System.IO.StreamWriter writer = System.IO.File.CreateText("config.xml");

            xs.Serialize(writer, MainConfig.propertyStore);

            writer.Flush();
            writer.Close();
        }

        public static void Register(IConfigurable configurable)
        {
            if(!MainConfig.registeredConfigurables.Contains(configurable))
            {
                MainConfig.registeredConfigurables.Add(configurable);
            }

            MainConfig.LoadConfiguration(configurable);
        }

        private static void LoadConfiguration(IConfigurable configurable)
        {
            if(!MainConfig.isInitialized)
            {
                MainConfig.Initialize();
            }

            foreach(PropertyInfo property in configurable.GetType().GetProperties())
            {
                Dictionary<string, object> currentStore = MainConfig.GetPropertyStoreForType(configurable.GetType());

                ConfigurableAttribute configurableAttribute = Attribute.GetCustomAttribute(property, typeof(ConfigurableAttribute)) as ConfigurableAttribute;

                /* Skip non-writable non-configurable properties. */
                if(property.CanWrite && configurableAttribute != null)
                {
                    property.SetValue(configurable, MainConfig.GetPropertyValue(property), null);
                }
            }
        }

        public static void SaveConfiguration(IConfigurable configurable)
        {
            if(!MainConfig.isInitialized)
            {
                MainConfig.Initialize();
            }

            Dictionary<string, object> currentStore = MainConfig.GetPropertyStoreForType(configurable.GetType());
            
            foreach(PropertyInfo property in configurable.GetType().GetProperties())
            {
                ConfigurableAttribute configurableAttribute = Attribute.GetCustomAttribute(property, typeof(ConfigurableAttribute)) as ConfigurableAttribute;

                /* Skip non-readable non-configurable properties. */
                if (property.CanWrite && configurableAttribute != null)
                {
                    currentStore[property.Name] = property.GetValue(configurable, null);
                }
            }
        }

        private static Dictionary<string, object> GetPropertyStoreForType(Type owner)
        {
            if (!MainConfig.propertyStore.ContainsKey(owner.Name))
            {
                MainConfig.propertyStore.Add(owner.Name, new Dictionary<string, object>());
            }

            return MainConfig.propertyStore[owner.Name];
        }

        public static object GetPropertyValue(PropertyInfo property)
        {
            Dictionary<string, object> currentStore = MainConfig.GetPropertyStoreForType(property.ReflectedType);

            ConfigurableAttribute configurableAttribute = Attribute.GetCustomAttribute(property, typeof(ConfigurableAttribute)) as ConfigurableAttribute;

            // Does the property store contain this property?
            if(currentStore.ContainsKey(property.Name))
            {
                object storeValue = currentStore[property.Name];

                // Do the types match?
                if(property.PropertyType.IsAssignableFrom(storeValue.GetType())){
                    return storeValue;
                }
                // Otherwise use one of the following default value modes
            }

            // The value is not in property store (or is not valid), but its default value is known.
            if(configurableAttribute.DefaultValue != null)
            {
                // Do the types match?
                if (property.PropertyType.IsAssignableFrom(configurableAttribute.DefaultValue.GetType()))
                {
                    return configurableAttribute.DefaultValue;
                }
                else
                {
                    throw new InvalidCastException("Default value for property \"" + property.ToString() + "\" is not compatible with its type.");
                }
            }
            // The  property is of a value type - use that type's default value.
            else if(property.PropertyType.IsValueType)
            {
                return Activator.CreateInstance(property.PropertyType);
            }
            // The property is of a reference type - use null.
            else if(configurableAttribute.UseEmptyInstanceAsDefault)
            {
                try{
                    return Activator.CreateInstance(property.PropertyType);
                }
                catch(Exception e)
                {
                    throw new InvalidOperationException("Could not create a default instance for property \"" + property.ToString() + "\", see innerException for details.", e);
                }
            }
            // The property is of a reference type and no default other default value could be created - use null.
            else
            {
                return null;
            }          
        }
    }
}
