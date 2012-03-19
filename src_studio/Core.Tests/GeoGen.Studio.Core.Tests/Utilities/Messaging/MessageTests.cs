namespace GeoGen.Studio.Utilities.Messaging
{
    using System;
    using System.Windows.Threading;

    using GeoGen.Studio.Utilities.Extensions;

    using NUnit.Framework;

    // ReSharper disable InconsistentNaming

    /// <summary>
    /// Tests for the <see cref="Message"/> class.
    /// </summary>
    [TestFixture]
    public class MessageTests
    {
        /// <summary>
        /// Tests that <see cref="Message.Send"/> delivers the message.
        /// </summary>
        [Test]
        public void Send_Message_Sends()
        {
            var messenger = new Messenger
            {
                Dispatcher = Dispatcher.CurrentDispatcher
            };

            // Message uses the default messenger initially
            Messenger.Instance = messenger;

            var message = new Message("text");

            bool thrown = false;
            messenger.MessageSent += delegate { thrown = true; };
            message.Send();

            messenger.Dispatcher.DoEvents();

            Assert.IsTrue(thrown);
        }

        /// <summary>
        /// Tests that <see cref="Message.DateTime"/> returns correct <see cref="DateTime"/>.
        /// </summary>
        [Test]
        public void DateTime_ReturnsCorrectDateTime()
        {
            var timeBefore = DateTime.Now;
            var message = new Message("foo");
            var timeAfter = DateTime.Now;

            bool result = timeBefore.CompareTo(message.DateTime) != 1;
            result &= timeAfter.CompareTo(message.DateTime) != -1;

            Assert.IsTrue(result);
        }

        /// <summary>
        /// Tests that string returned by <see cref="Message.DateTimeString"/> is string representation of its <see cref="Message.DateTime"/>.
        /// </summary>
        [Test]
        public void DateTimeString_ReturnsCorrectString()
        {
            var message = new Message("foo");

            var parsedDateTime = DateTime.Parse(message.DateTimeString);

            var difference = message.DateTime - parsedDateTime;

            // Allow 60 second error (DateTime.Parse doesn't parse seconds)
            Assert.IsTrue(difference.TotalSeconds < 60);
        }

        /// <summary>
        /// Tests that <see cref="Message.Text"/> works.
        /// </summary>
        [Test]
        public void Text_ReturnsCorrectText()
        {
            var message = new Message("foo");

            Assert.AreEqual(message.Text, "foo");
        }

        /// <summary>
        /// Tests that <see cref="Message.Type"/> works.
        /// </summary>
        [Test]
        public void Type_ReturnsCorrectType()
        {
            var message = new Message("foo", MessageType.Warning);

            Assert.AreEqual(message.Type, MessageType.Warning);
        }
    }
}
