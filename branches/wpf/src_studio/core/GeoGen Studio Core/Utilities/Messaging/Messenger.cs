using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Threading;
using GeoGen.Studio;

namespace GeoGen.Studio.Utilities.Messaging
{
    public static class Messenger
    {
        public static event MessageThrownEventHandler MessageThrown;

        private static List<Message> messageHistory = new List<Message>();
        public static IEnumerable<Message> MessageHistory
        {
            get
            {
                return Messenger.messageHistory.AsReadOnly();
            }
        }

        public static void ThrowMessage(Message message)
        {
            Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal, (Action)delegate()
            {
                Messenger.messageHistory.Add(message);

                if(MessageThrown != null)
                {
                    Messenger.MessageThrown(null, new MessageThrownEventArgs(message));
                }
            });
        }
    }
}
