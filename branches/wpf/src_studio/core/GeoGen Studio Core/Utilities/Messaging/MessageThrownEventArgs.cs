using System;

namespace GeoGen.Studio.Utilities.Messaging
{
    /// <summary>
    /// Provides data for <see cref="Messenger.MessageThrown"/> event.
    /// </summary>
    public class MessageThrownEventArgs : EventArgs
    {
        /// <summary>
        /// Gets or sets the message.
        /// </summary>
        /// <value>The message.</value>
        public Message Message { get; private set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="MessageThrownEventArgs"/> class.
        /// </summary>
        /// <param name="message">The message.</param>
        public MessageThrownEventArgs(Message message)
        {
            this.Message = message;
        }
    }
}
