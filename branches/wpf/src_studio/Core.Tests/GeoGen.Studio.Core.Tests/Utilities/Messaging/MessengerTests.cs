namespace GeoGen.Studio.Utilities.Messaging
{
	using System.Linq;
	using System.Windows.Threading;

	using GeoGen.Studio.Utilities.Extensions;

	using NUnit.Framework;

	// ReSharper disable InconsistentNaming

	[TestFixture]
	class MessengerTests
	{
		[SetUp]
		public void SetUp()
		{
			Messenger.Dispatcher = Dispatcher.CurrentDispatcher;
			Messenger.ClearHistory();
		}

		[Test]
		public void ThrowMessage_MessageThrown_Throws()
		{
			bool thrown = true;

			var message = new Message("text");

			Messenger.MessageThrown += delegate { thrown = true; };

			Messenger.ThrowMessage(message);

			Messenger.Dispatcher.DoEvents();

			Assert.IsTrue(thrown);
		}

		[Test]
		public void ThrowMessage_MessageThrown_SameMessage()
		{
			var message = new Message("text2");
			Message thrownMessage = null;

			Messenger.MessageThrown += delegate(object o, MessageThrownEventArgs args)
			{
				thrownMessage = args.Message;
			};

			Messenger.ThrowMessage(message);

			Messenger.Dispatcher.DoEvents();

			Assert.AreSame(message, thrownMessage);
		}

		[Test]
		public void ThrowMessage_MessageThrown_AppearsInHistory()
		{
			var message = new Message("testThrowMessage");

			Messenger.ThrowMessage(message);

			Assert.IsTrue(Messenger.MessageHistory.Contains(message));
		}

		[Test]
		public void ThrowMessage_TwoMessages_InOrderInHistory()
		{
			var message1 = new Message("testThrowMessage1");
			var message2 = new Message("testThrowMessage2");

			Messenger.ThrowMessage(message1);
			Messenger.ThrowMessage(message2);

			int index1 = Messenger.MessageHistory.ToList().IndexOf(message1);
			int index2 = Messenger.MessageHistory.ToList().IndexOf(message2);

			Assert.Less(index1, index2);
		}

		[Test]
		public void MessageHistory_NoMessageThrown_Empty()
		{
			// This relies on the history being cleared in set-up.
			Assert.That(Messenger.MessageHistory, Is.EquivalentTo(Enumerable.Empty<Message>()));
		}
	}
}
