using System;
using System.Collections.Generic;
using System.Xml.Serialization;

namespace GeoGen.Studio.Utilities.Collections
{
    /// <summary>
    /// Serializable collection of <see cref="Type"/> objects.
    /// </summary>
    public class TypeCollection: List<Type>, IXmlSerializable
    {
        public System.Xml.Schema.XmlSchema GetSchema()
        {
            return null;
        }

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
                reader.ReadStartElement("type");
                string typeName = reader.ReadString();
                reader.ReadEndElement();

                Type type = null;
                try
                {
                    type = Type.GetType(typeName);
                }
                catch(Exception)
                {
                    continue;
                }

                if (!this.Contains(type))
                {
                    this.Add(type);
                }
            }
            reader.ReadEndElement();
        }

        public void WriteXml(System.Xml.XmlWriter writer)
        {
            foreach (Type type in this)
            {
                writer.WriteStartElement("type");
                writer.WriteValue(type.AssemblyQualifiedName);
                writer.WriteEndElement(); // </type>
            }
        }
    }
}
