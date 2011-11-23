using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Threading;

namespace GeoGen.Studio.Utilities.Messaging
{
    /// <summary>
    /// Provides service for broadcasting <see cref="Message">messages</see> to potentially unknown recipients.
    /// </summary>
    public static class Messenger
    {
        private static Dispatcher dispatcher;

        public static Dispatcher Dispatcher
        {
            get
            {
                if (Messenger.dispatcher != null)
                {
                    return Messenger.dispatcher;
                }
                else
                {
                    return Application.Current.Dispatcher;
                }
            }

            internal set
            {
                Messenger.dispatcher = value;
            }
        }
        
        /// <summary>
        /// Occurs when a <see cref="Message"/> is broadcasted.
        /// </summary>
        public static event MessageThrownEventHandler MessageThrown;

        private static readonly List<Message> messageHistory = new List<Message>();
        /// <summary>
        /// History of all <see cref="Message">messages</see> broadcatested in order from the oldest.
        /// </summary>
        /// <value>The message history.</value>
        public static IEnumerable<Message> MessageHistory
        {
            get
            {
                return Messenger.messageHistory.AsReadOnly();
            }
        }

        /// <summary>
        /// Broadcasts a <see cref="Message"/> to all listening.
        /// </summary>
        /// <param name="message">The message.</param>
        public static void ThrowMessage(Message message)
        {
            Messenger.Dispatcher.Invoke(DispatcherPriority.Normal, (Action)delegate()
            {
                Messenger.messageHistory.Add(message);

                if (Messenger.MessageThrown != null)
                {
                    Messenger.MessageThrown(null, new MessageThrownEventArgs(message));
                }
            });
        }

        internal static void ClearHistory()
        {
            Messenger.messageHistory.Clear();
        }
    }
}
