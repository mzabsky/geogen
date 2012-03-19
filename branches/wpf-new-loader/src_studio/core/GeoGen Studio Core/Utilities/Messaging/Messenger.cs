namespace GeoGen.Studio.Utilities.Messaging
{
    using System;
    using System.Collections.Generic;
    using System.Windows;
    using System.Windows.Threading;

    /// <summary>
    /// Provides service for broadcasting <see cref="Message">messages</see> to potentially unknown recipients.
    /// </summary>
    public class Messenger
    {
        /// <summary>
        /// List of all messages delivered with this instance.
        /// </summary>
        private readonly List<Message> messageHistory = new List<Message>();

        /// <summary>
        /// Singleton instance.
        /// </summary>
        private static Messenger instance;

        /// <summary>
        /// <see cref="Dispatcher"/> to which the messages are delivered.
        /// </summary>
        private Dispatcher dispatcher;

        /// <summary>
        /// Prevents a default instance of the <see cref="Messenger"/> class from being created.
        /// </summary>
        internal Messenger()
        {
        }

        /// <summary>
        /// Occurs when a <see cref="Message"/> is broadcasted.
        /// </summary>
        public event MessageThrownEventHandler MessageSent;

        /// <summary>
        /// Gets the instance.
        /// </summary>
        public static Messenger Instance
        {
            get
            {
                return Messenger.instance ?? new Messenger();
            }
            internal set
            {
                Messenger.instance = value;
            }
        }

        /// <summary>
        /// <see cref="Dispatcher"/> to whose thread the message is delivered.
        /// </summary>
        public Dispatcher Dispatcher
        {
            get
            {
                return this.dispatcher ?? Application.Current.Dispatcher;
            }

            internal set
            {
                this.dispatcher = value;
            }
        }

        /// <summary>
        /// History of all <see cref="Message">messages</see> broadcatested in order from the oldest.
        /// </summary>
        /// <value>The message history.</value>
        public IEnumerable<Message> MessageHistory
        {
            get
            {
                return this.messageHistory.AsReadOnly();
            }
        }

        /// <summary>
        /// Broadcasts a <see cref="Message"/> to all listening receivers.
        /// </summary>
        /// <param name="message">The message.</param>
        public void SendMessage(Message message)
        {
            this.Dispatcher.Invoke(DispatcherPriority.Normal, (Action)delegate
            {
                this.messageHistory.Add(message);

                if (this.MessageSent != null)
                {
                    this.MessageSent(null, new MessageThrownEventArgs(message));
                }
            });
        }
    }
}
