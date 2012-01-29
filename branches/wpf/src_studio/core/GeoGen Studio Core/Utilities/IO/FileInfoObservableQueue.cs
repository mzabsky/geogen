namespace GeoGen.Studio.Utilities.IO
{
    using GeoGen.Studio.Utilities.Collections;
    using System.IO;
    using System.Linq;    
    using System.Xml.Serialization;    

    /// <summary>
    /// Observable collection of files. Each file can be only once in the collection. The collection has total capacity of 20 entries.
    /// </summary>
    public class FileInfoObservableQueue: UniqueObservableQueue<FileInfo>, IXmlSerializable
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="FileInfoObservableQueue"/> class.
        /// </summary>
        public FileInfoObservableQueue(): base(new DelegateEqualityComparer<FileInfo>((a, b) => a.FullName == b.FullName))
        {         
        }

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
                string fileName = reader.ReadString();
                reader.ReadEndElement();

                try
                {
                    this.Add(new FileInfo(fileName));   
                }
                catch(IOException)
                {                    
                }
            }
            reader.ReadEndElement();            
        }

        /// <exclude />
        public void WriteXml(System.Xml.XmlWriter writer)
        {
            // write the elements in  reverse order (so the XML can be read as a queue)
            var reversed = this.ToList();
            reversed.Reverse();

            foreach (FileInfo fileInfo in reversed)
            {
                writer.WriteStartElement("item");
                writer.WriteValue(fileInfo.FullName);
                writer.WriteEndElement();
            }
        }
    }
}
