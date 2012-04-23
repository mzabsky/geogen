using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.IO;
using System.Reflection;
using System.Windows;
using GeoGen.Studio.Utilities.IO;

namespace GeoGen.Studio.Utilities.Persistence
{
    /// <summary>
    /// Offers a simple way to persistently store properties of objects on a per-type basis.
    /// </summary>
    public static class MainConfig
    {
        private const BindingFlags SupportedPropertyBindingFlags = BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.FlattenHierarchy;

        private static PropertyStore propertyStore;
        private static readonly List<object> registeredConfigurables = new List<object>();
        private static readonly List<Type> registeredStaticTypes = new List<Type>();

        static MainConfig()
        {
            // Load the configuration  data from the config.xml file.
            System.Xml.Serialization.XmlSerializer xs = new System.Xml.Serialization.XmlSerializer(typeof(PropertyStore));

            try
            {
                using (StreamReader reader = File.OpenText("config.xml"))

                    MainConfig.propertyStore = xs.Deserialize(reader) as PropertyStore;
            }
            catch
            {
                MainConfig.propertyStore = new PropertyStore();
            }

            // Save the config.xml when the application is terminating.
            Application.Current.Exit += delegate
            {
                MainConfig.SaveConfigurations();
            };
        }

        private static void SaveConfigurations()
        {
            foreach (object configurable in MainConfig.registeredConfigurables)
            {
                MainConfig.SaveConfiguration(configurable);
            }

            foreach (Type type in MainConfig.registeredStaticTypes)
            {
                MainConfig.SaveConfigurationForStaticType(type);
            }

            System.Xml.Serialization.XmlSerializer xs = new System.Xml.Serialization.XmlSerializer(typeof(PropertyStore));

            using (StreamWriter writer = File.CreateText("config.xml"))
            {
                xs.Serialize(writer, MainConfig.propertyStore);

                writer.Flush();
                writer.Close();
            }
        }

        /// <summary>
        /// Registers a configurable object for persistent storage. The object's properties will be updated from the storage upon registraation.
        /// When the application shuts down, its properties will be saved into the storage.
        /// </summary>
        /// <param name="configurable">The configurable.</param>
        public static void Register(object configurable)
        {
            if(!MainConfig.registeredConfigurables.Contains(configurable))
            {
                MainConfig.registeredConfigurables.Add(configurable);
            }

            MainConfig.LoadConfiguration(configurable);
        }

        public static void RegisterStaticType(Type type)
        {
            // The type must be static.
            Contract.Assert(type.IsAbstract && type.IsSealed);

            if (!MainConfig.registeredStaticTypes.Contains(type))
            {
                MainConfig.registeredStaticTypes.Add(type);
            }

            MainConfig.LoadConfigurationForStaticType(type);
        }

        private static void LoadConfiguration(object configurable)
        {
            foreach (PropertyInfo property in configurable.GetType().GetProperties(MainConfig.SupportedPropertyBindingFlags))
            {
                PersistentAttribute configurableAttribute = Attribute.GetCustomAttribute(property, typeof(PersistentAttribute)) as PersistentAttribute;

                /* Skip non-writable non-configurable properties. */
                if(property.CanWrite && configurableAttribute != null)
                {
                    property.SetValue(configurable, MainConfig.GetPropertyValue(property), null);
                }
            }
        }

        private static void LoadConfigurationForStaticType(Type type)
        {
            // The type must be static.
            Contract.Assert(type.IsAbstract && type.IsSealed);

            foreach (PropertyInfo property in type.GetProperties(MainConfig.SupportedPropertyBindingFlags))
            {
                PersistentAttribute configurableAttribute = Attribute.GetCustomAttribute(property, typeof(PersistentAttribute)) as PersistentAttribute;

                /* Skip non-writable non-configurable properties. */
                if (property.CanWrite && configurableAttribute != null)
                {
                    object o = MainConfig.GetPropertyValue(property);
                    property.SetValue(type, MainConfig.GetPropertyValue(property), null);
                }
            }
        }

        /// <summary>
        /// Saves properties of the configurable object to the persistent storage. The object will be <see cref="MainConfig.Register">registered</see>, if it is not already.
        /// </summary>
        /// <param name="configurable">The configurable.</param>
        public static void SaveConfiguration(object configurable)
        {
            foreach (PropertyInfo property in configurable.GetType().GetProperties(MainConfig.SupportedPropertyBindingFlags))
            {
                PersistentAttribute configurableAttribute = Attribute.GetCustomAttribute(property, typeof(PersistentAttribute)) as PersistentAttribute;

                /* Skip non-readable non-configurable properties. */
                if (property.CanWrite && configurableAttribute != null)
                {
                    MainConfig.SavePropertyValue(property, property.GetValue(configurable, null));
                }
            }
        }

        public static void SaveConfigurationForStaticType(Type type)
        {
            // The type must be static.
            Contract.Assert(type.IsAbstract && type.IsSealed);

            foreach (PropertyInfo property in type.GetProperties(MainConfig.SupportedPropertyBindingFlags))
            {
                PersistentAttribute configurableAttribute = Attribute.GetCustomAttribute(property, typeof(PersistentAttribute)) as PersistentAttribute;

                /* Skip non-readable non-configurable properties. */
                if (property.CanWrite && configurableAttribute != null)
                {
                    MainConfig.SavePropertyValue(property, property.GetValue(null, null));
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

            PersistentAttribute configurableAttribute = Attribute.GetCustomAttribute(property, typeof(PersistentAttribute)) as PersistentAttribute ?? new PersistentAttribute();

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

                throw new InvalidCastException(string.Format("Default value for property \"{0}\" is not compatible with its type.", property));
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
                    throw new InvalidOperationException(string.Format("Could not create a default instance for property \"{0}\", see innerException for details.", property), e);
                }
            }
            
            // The property is of a reference type and no default other default value could be created - use null.
            return null;
        }

        /// <summary>
        /// Sets the property value that will be written into the configuration file next time it is updated.
        /// </summary>
        /// <param name="property">The property.</param>
        /// <param name="value">The value.</param>
        static public void SavePropertyValue(PropertyInfo property, object value)
        {
            Dictionary<string, object> currentStore = MainConfig.GetPropertyStoreForType(property.DeclaringType);

            currentStore[property.Name] = value;
        }
    }
}
