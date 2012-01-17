namespace GeoGen.Studio.Utilities.Messaging
{
    using System.Linq;
    using System.Windows.Threading;

    using GeoGen.Studio.Utilities.Extensions;

    using NUnit.Framework;

    // ReSharper disable InconsistentNaming

    /// <summary>
    /// Tests for the <see cref="Messenger"/> class.
    /// </summary>
    [TestFixture]
    class MessengerTests
    {
        /// <summary>
        /// Creates the messenger for this testing suite.
        /// </summary>
        /// <returns>Messenger instance with correctly initialized <see cref="Messenger.Dispatcher"/></returns>
        public Messenger createMessenger()
        {
            return new Messenger
            {
                Dispatcher = Dispatcher.CurrentDispatcher
            };
        }

        /// <summary>
        /// Tests that instance getter really returns valids instance.
        /// </summary>
        [Test]
        public void Instance_ReturnsInstance()
        {
            Assert.IsInstanceOf<Messenger>(Messenger.Instance);
        }

        /// <summary>
        /// Tests that <see cref="Messenger.SendMessage"/> delivers the message.
        /// </summary>
        [Test]
        public void SendMessage_MessageSent_Sends()
        {
            var messenger = this.createMessenger();
            var message = new Message("text");

            bool thrown = true;
            messenger.MessageSent += delegate { thrown = true; };
            messenger.SendMessage(message);

            messenger.Dispatcher.DoEvents();

            Assert.IsTrue(thrown);
        }

        /// <summary>
        /// Tests that <see cref="Messenger.SendMessage"/> delivers the message correctly (that it delivers the same message instance).
        /// </summary>
        [Test]
        public void SendMessage_MessageSent_SameMessage()
        {
            var messenger = this.createMessenger();
            var message = new Message("text2");
            Message thrownMessage = null;

            messenger.MessageSent += delegate(object o, MessageThrownEventArgs args)
            {
                thrownMessage = args.Message;
            };

            messenger.SendMessage(message);

            messenger.Dispatcher.DoEvents();

            Assert.AreSame(message, thrownMessage);
        }

        /// <summary>
        /// Tests that <see cref="Messenger.SendMessage"/> adds the message to <see cref="Messenger.MessageHistory"/>.
        /// </summary>
        [Test]
        public void SendMessage_MessageSent_AppearsInHistory()
        {
            var messenger = this.createMessenger();
            var message = new Message("testThrowMessage");

            messenger.SendMessage(message);

            Assert.IsTrue(messenger.MessageHistory.Contains(message));
        }

        /// <summary>
        /// Tests that sent messages appear in history in orer in which they were sent.
        /// </summary>
        [Test]
        public void SendMessage_TwoMessages_InOrderInHistory()
        {
            var messenger = this.createMessenger();
            var message1 = new Message("testThrowMessage1");
            var message2 = new Message("testThrowMessage2");

            messenger.SendMessage(message1);
            messenger.SendMessage(message2);

            int index1 = messenger.MessageHistory.ToList().IndexOf(message1);
            int index2 = messenger.MessageHistory.ToList().IndexOf(message2);

            Assert.Less(index1, index2);
        }

        /// <summary>
        /// Tests that message history is empty when no messages were sent.
        /// </summary>
        [Test]
        public void MessageHistory_NoMessageSent_Empty()
        {
            var messenger = this.createMessenger();
            
            // This relies on the history being cleared in set-up.
            Assert.That(messenger.MessageHistory, Is.EquivalentTo(Enumerable.Empty<Message>()));
        }
    }
}
