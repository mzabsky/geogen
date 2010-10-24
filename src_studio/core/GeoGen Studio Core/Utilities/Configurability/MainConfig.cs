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

            if(!propertyStore.ContainsKey(configurable.GetType().ToString())){
                return;
            }

            Dictionary<string, object> currentStore = MainConfig.propertyStore[configurable.GetType().ToString()];

            foreach(PropertyInfo property in configurable.GetType().GetProperties())
            {
                ConfigurableAttribute configurableAttribute = Attribute.GetCustomAttribute(property, typeof(ConfigurableAttribute)) as ConfigurableAttribute;

                /* Skip non-writable non-configurable properties. */
                if(property.CanWrite && configurableAttribute != null)
                {
                    /* Does the property store contain this property? */
                    if(currentStore.ContainsKey(property.Name))
                    {
                        object storeValue = currentStore[property.Name];

                        /* Do the types match? */
                        if(property.PropertyType.IsAssignableFrom(storeValue.GetType())){
                            property.SetValue(configurable, storeValue, null);
                        }
                    }
                }
            }
        }

        public static void SaveConfiguration(IConfigurable configurable)
        {
            if(!MainConfig.isInitialized)
            {
                MainConfig.Initialize();
            }

            string confiurableName = configurable.GetType().ToString();

            if (!MainConfig.propertyStore.ContainsKey(confiurableName))
            {
                MainConfig.propertyStore.Add(confiurableName, new Dictionary<string, object>());
            }

            Dictionary<string, object> currentStore = MainConfig.propertyStore[confiurableName];

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
    }
}
