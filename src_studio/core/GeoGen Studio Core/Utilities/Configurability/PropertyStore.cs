using System;
using System.Collections.Generic;
using System.Xml.Serialization;
using System.ComponentModel;

namespace GeoGen.Studio.Utilities.Configurability
{
    /* Exclude this class from both IntelliSense and documentation, it is of no purpose for anyone but MainConfig class.
     * It is public only because XML serializer needs it to be. */

    /// <summary>
    /// Serializable two-level dictionary tailored for purposes of MainConfig Class. 
    /// </summary>
    /// <exclude/>
    [XmlRoot("PropertyStore")]
    [EditorBrowsable(System.ComponentModel.EditorBrowsableState.Never)]
    public class PropertyStore : Dictionary<string /* owner type */, Dictionary<string /* property name */, object /* property value */>>, IXmlSerializable
    {
        /// <exclude />
        public System.Xml.Schema.XmlSchema GetSchema()
        {
            // Required by IXmlSerializable, but not necessary
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

                reader.ReadStartElement("owner");
                string ownerName = reader.ReadString();
                reader.ReadEndElement();

                reader.ReadStartElement("property");
                string propertyName = reader.ReadString();
                reader.ReadEndElement();

                reader.ReadStartElement("type");
                Type valueType = Type.GetType(reader.ReadString());
                reader.ReadEndElement();

                // Attempt to deserialize the value type.
                XmlSerializer valueSerializer;
                if (valueType != null)
                {
                    valueSerializer = new XmlSerializer(valueType);
                }
                else
                {
                    /* Deserialization of this property's value type, try to resume deserialization on next item. */
                    reader.Skip();
                    reader.ReadEndElement(); // </type>
                    reader.Skip(); // <value></value>
                    reader.ReadEndElement(); // </item>
                    reader.MoveToContent();

                    continue;
                }

                reader.ReadStartElement("value");

                object value;
                try
                {
                    value = valueSerializer.Deserialize(reader);
                }
                catch
                {
                    // Deserialization of this property's value failed, try to resume deserialization on next item.
                    reader.Skip();
                    reader.ReadEndElement(); // </value>
                    reader.ReadEndElement(); // </item>
                    reader.MoveToContent();

                    continue;
                }

                reader.ReadEndElement(); // </value>
                reader.ReadEndElement(); // </item>
                reader.MoveToContent(); 

                if(!this.ContainsKey(ownerName))
                {
                    this.Add(ownerName, new Dictionary<string, object>());
                }

                if(!this[ownerName].ContainsKey(propertyName))
                {
                    this[ownerName].Add(propertyName, value);
                }
                else
                {
                    this[ownerName][propertyName] = value;
                }
            }
            reader.ReadEndElement();
        }

        /// <exclude />
        public void WriteXml(System.Xml.XmlWriter writer)
        {
            foreach (string ownerName in this.Keys)
            {
                Dictionary<string, object> plugInProperties = this[ownerName];

                foreach(string propertyName in plugInProperties.Keys){
                    object currentValue = plugInProperties[propertyName];

                    XmlSerializer valueSerializer = new XmlSerializer(currentValue.GetType());

                    writer.WriteStartElement("item");

                    writer.WriteStartElement("owner");
                    writer.WriteValue(ownerName);
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
