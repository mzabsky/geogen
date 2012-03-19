namespace GeoGen.Studio.Utilities.Messaging
{
    using System;

    /// <summary>
    /// Represents a text message with additional information about its severity and occurence time. This type is immutable.
    /// </summary>
    public class Message
    {
        /// <summary>
        /// Text of the message.
        /// </summary>
        private readonly string text;

        /// <summary>
        /// Type of the message.
        /// </summary>
        private readonly MessageType type;

        /// <summary>
        /// Time when the message was thrown.
        /// </summary>
        private readonly DateTime dateTime;

        /// <summary>
        /// Initializes a new instance of the <see cref="Message"/> class.
        /// </summary>
        /// <param name="text">The text.</param>
        /// <param name="type">The type.</param>
        public Message(string text, MessageType type = MessageType.Message)
        {
            this.text = text;
            this.type = type;
            this.dateTime = DateTime.Now;
        }

        /// <summary>
        /// Gets time of occurence.
        /// </summary>
        /// <value>The time of occurence.</value>
        public DateTime DateTime
        {
            get
            {
                return this.dateTime;
            }
        }

        /// <summary>
        /// Gets short string representation of time of occurence.
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
        /// Gets text of the message.
        /// </summary>
        /// <value>The text.</value>
        public string Text
        {
            get
            {
                return this.text;
            }
        }

        /// <summary>
        /// Gets type of the message.
        /// </summary>
        /// <value>The type.</value>
        public MessageType Type 
        {
            get
            {
                return this.type;
            }
        }

        /// <summary>
        /// Sends this message using the <see cref="Messenger"/>.
        /// </summary>
        public void Send()
        {
            Messenger.Instance.SendMessage(this);
        }

        /// <summary>
        /// Returns a <see cref="System.String"/> that represents this instance.
        /// </summary>
        /// <returns>
        /// A <see cref="System.String"/> that represents this instance.
        /// </returns>
        public override string ToString()
        {
            return this.Type.ToString() + ": " + this.Text;
        }
    }
}
