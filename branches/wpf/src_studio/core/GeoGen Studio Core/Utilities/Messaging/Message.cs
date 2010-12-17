using System;

namespace GeoGen.Studio.Utilities.Messaging
{
    public class Message
    {
        public string Text {get; set;}
        public MessageType Type {get; set;}
        public DateTime DateTime { get; set; }
        public string DateTimeString
        {
            get
            {
                return this.DateTime.ToShortTimeString();
            }
        }

        public Message(string text, MessageType type = MessageType.Message)
        {
            this.Text = text;
            this.Type = type;
            this.DateTime = DateTime.Now;
        }
    }
}
