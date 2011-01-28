using System;

namespace GeoGen.Studio.Utilities.Messaging
{
    /// <summary>
    /// Represents a text message with additional information about its severity and occurence time.
    /// </summary>
    public class Message
    {
        /// <summary>
        /// Text of the message.
        /// </summary>
        /// <value>The text.</value>
        public string Text {get; set;}

        /// <summary>
        /// Type of the message.
        /// </summary>
        /// <value>The type.</value>
        public MessageType Type {get; set;}

        /// <summary>
        /// Time of occurence.
        /// </summary>
        /// <value>The time of occurence.</value>
        public DateTime DateTime { get; set; }

        /// <summary>
        /// Short string representation of time of occurence.
        /// </summary>
        /// <value>The time string.</value>
        public string DateTimeString
        {
            get
            {
                return this.DateTime.ToShortTimeString();
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Message"/> class.
        /// </summary>
        /// <param name="text">The text.</param>
        /// <param name="type">The type.</param>
        public Message(string text, MessageType type = MessageType.Message)
        {
            this.Text = text;
            this.Type = type;
            this.DateTime = DateTime.Now;
        }
    }
}
