using System;
using System.Collections.Generic;
using System.Xml.Serialization;
using System.ComponentModel;
using System.Reflection;

namespace GeoGen.Studio.Utilities.Configurability
{
    /* Exclude this class from both IntelliSense and documentation, it is of no purpose for anyone but MainConfig class.
     * It is public only because XML serializer needs it to be. */

    /// <exclude />
    [XmlRoot("PropertyStore")]
    [EditorBrowsable(System.ComponentModel.EditorBrowsableState.Never)]
    public class PropertyStore : Dictionary<string, Dictionary<string, object>>, IXmlSerializable
    {
        public System.Xml.Schema.XmlSchema GetSchema()
        {
            return null;
        }

        /// <exclude />
        public void ReadXml(System.Xml.XmlReader reader)
        {
            bool wasEmpty = reader.IsEmptyElement;

            reader.Read();

            if (wasEmpty)
            {
                return;
            }

            while (reader.NodeType != System.Xml.XmlNodeType.EndElement)
            {
                reader.ReadStartElement("item");

                reader.ReadStartElement("plugIn");
                string plugInName = reader.ReadString();
                reader.ReadEndElement();

                reader.ReadStartElement("property");
                string propertyName = reader.ReadString();
                reader.ReadEndElement();

                reader.ReadStartElement("type");
                Type valueType = Type.GetType(reader.ReadString());
                reader.ReadEndElement();

                /* If the type failed to initialize, don't create the XmlSerializer object, the deserialization will
                 * then crash on NullReferenceException and the catch statement will make sure the item is ignored. */
                XmlSerializer valueSerializer = null;
                if (valueType != null)
                {
                    valueSerializer = new XmlSerializer(valueType);
                }

                reader.ReadStartElement("value");

                object value = null;
                try
                {
                    value = valueSerializer.Deserialize(reader);
                }
                catch
                {
                    /* Try to resume deserialization on next item. */
                    reader.Skip();
                    reader.ReadEndElement(); // </value>
                    reader.ReadEndElement(); // </item>
                    reader.MoveToContent();

                    continue;
                }

                reader.ReadEndElement(); // </value>
                reader.ReadEndElement(); // </item>
                reader.MoveToContent(); 

                if(!this.ContainsKey(plugInName))
                {
                    this.Add(plugInName, new Dictionary<string, object>());
                }

                if(!this[plugInName].ContainsKey(propertyName))
                {
                    this[plugInName].Add(propertyName, value);
                }
                else
                {
                    this[plugInName][propertyName] = value;
                }
            }
            reader.ReadEndElement();
        }

        /// <exclude />
        public void WriteXml(System.Xml.XmlWriter writer)
        {
            XmlSerializer keySerializer = new XmlSerializer(typeof(Type));

            foreach (string plugInName in this.Keys)
            {
                Dictionary<string, object> plugInProperties = this[plugInName];

                foreach(string propertyName in plugInProperties.Keys){
                    object currentValue = plugInProperties[propertyName];

                    XmlSerializer valueSerializer = new XmlSerializer(currentValue.GetType());

                    writer.WriteStartElement("item");

                    writer.WriteStartElement("plugIn");
                    writer.WriteValue(plugInName);
                    writer.WriteEndElement();

                    writer.WriteStartElement("property");
                    writer.WriteValue(propertyName);
                    writer.WriteEndElement();

                    writer.WriteStartElement("type");
                    writer.WriteValue(currentValue.GetType().AssemblyQualifiedName);
                    writer.WriteEndElement();

                    writer.WriteStartElement("value");
                    valueSerializer.Serialize(writer, currentValue);
                    writer.WriteEndElement();

                    writer.WriteEndElement(); // </item>
                }                
            }
        }
    }
}
