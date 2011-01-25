using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Windows;

namespace GeoGen.Studio.Utilities.Configurability
{
    /// <summary>
    /// Offers a simple way to persistently store properties of objects on a per-type basis.
    /// </summary>
    public static class MainConfig
    {
        private static bool isInitialized;
        private static PropertyStore propertyStore;
        private static readonly List<IConfigurable> registeredConfigurables = new List<IConfigurable>();

        private static void Initialize()
        {
            /* Load the configuration  data from the config.xml file. */
            System.Xml.Serialization.XmlSerializer xs = new System.Xml.Serialization.XmlSerializer(typeof(PropertyStore));

            try
            {
                using(StreamReader reader = System.IO.File.OpenText("config.xml"))

                MainConfig.propertyStore = xs.Deserialize(reader) as PropertyStore;
            }
            catch
            {
                MainConfig.propertyStore = new PropertyStore();
            }

            /* Save the config.xml when the application is terminating. */
            Application.Current.Exit += delegate
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

        /// <summary>
        /// Registers a configurable object for persistent storage. The object's properties will be updated from the storage upon registraation.
        /// When the application shuts down, its properties will be saved into the storage.
        /// </summary>
        /// <param name="configurable">The configurable.</param>
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
                ConfigurableAttribute configurableAttribute = Attribute.GetCustomAttribute(property, typeof(ConfigurableAttribute)) as ConfigurableAttribute;

                /* Skip non-writable non-configurable properties. */
                if(property.CanWrite && configurableAttribute != null)
                {
                    property.SetValue(configurable, MainConfig.GetPropertyValue(property), null);
                }
            }
        }

        /// <summary>
        /// Saves properties of the configurable object to the persistent storage. The object will be <see cref="MainConfig.Register">registered</see>, if it is not already.
        /// </summary>
        /// <param name="configurable">The configurable.</param>
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

        /// <summary>
        /// Gets value of one specific property. If the property is not found on the storage, a default value according to its attributes will be used.
        /// </summary>
        /// <param name="property">The property.</param>
        /// <returns></returns>
        public static object GetPropertyValue(PropertyInfo property)
        {
            Dictionary<string, object> currentStore = MainConfig.GetPropertyStoreForType(property.ReflectedType);

            ConfigurableAttribute configurableAttribute = Attribute.GetCustomAttribute(property, typeof(ConfigurableAttribute)) as ConfigurableAttribute ?? new ConfigurableAttribute();

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

                throw new InvalidCastException("Default value for property \"" + property + "\" is not compatible with its type.");
            }
            
            // The  property is of a value type - use that type's default value.
            if(property.PropertyType.IsValueType)
            {
                return Activator.CreateInstance(property.PropertyType);
            }
            
            // The property is of a reference type - use null.
            if(configurableAttribute.UseEmptyInstanceAsDefault)
            {
                try{
                    return Activator.CreateInstance(property.PropertyType);
                }
                catch(Exception e)
                {
                    throw new InvalidOperationException("Could not create a default instance for property \"" + property + "\", see innerException for details.", e);
                }
            }
            
            // The property is of a reference type and no default other default value could be created - use null.
            return null;
        }
    }
}
