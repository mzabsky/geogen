using System;

namespace GeoGen.Studio.Utilities.Messaging
{
    public class MessageThrownEventArgs : EventArgs
    {
        public Message Message { get; private set; }

        public MessageThrownEventArgs(Message message)
        {
            this.Message = message;
        }
    }
}
