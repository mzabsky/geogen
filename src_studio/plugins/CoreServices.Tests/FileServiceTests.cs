namespace GeoGen.Studio.PlugIns.Services
{
    using System;

    using NUnit.Framework;

    // ReSharper disable InconsistentNaming

    /// <summary>
    /// Tests for the <see cref="FileService"/> class.
    /// </summary>
    [TestFixture]
    public class FileServiceTests
    {
        /// <summary>
        /// Tests that RecentFiles returns the same instance of the view for two consecutive calls.
        /// </summary>
        [Test]
        public void RecentFiles_TwoCalls_ReturnsSameInstances()
        {
            var service = new FileService();

            Assert.AreSame(service.RecentFiles, service.RecentFiles);
        }

        /// <summary>
        /// Tests that OnCreated returns session with null FileName.
        /// </summary>
        [Test]
        public void OnCreated_Always_ReturnsUnboundSession()
        {
            var service = new FileService();

            var result = service.OnCreated(this, "*.txt");

            Assert.IsFalse(result.IsBound);
        }

        /// <summary>
        /// Tests that OnCreated returns session with correct FileName.
        /// </summary>
        [Test]
        public void OnCreated_Always_SetsCorrectFileName()
        {
            var service = new FileService();

            var result = service.OnCreated(this, "*.txt");

            Assert.AreEqual("*.txt", result.FileName);
        }

        /// <summary>
        /// Tests that OnCreated returns session which is open.
        /// </summary>
        [Test]
        public void OnCreated_Always_ReturnsOpenSession()
        {
            var service = new FileService();

            var result = service.OnCreated(this, "*.txt");

            Assert.IsFalse(result.IsClosed);
        }

        /// <summary>
        /// Tests that OnCreated returns session which is not unsaved.
        /// </summary>
        [Test]
        public void OnCreated_Always_ReturnsNotUnsavedSession()
        {
            var service = new FileService();

            var result = service.OnCreated(this, "*.txt");

            Assert.IsFalse(result.IsUnsaved);
        }

        /// <summary>
        /// Thats that OnCreated passes the session as event sender.
        /// </summary>
        [Test]
        public void OnCreated_Created_HasCorrectSenderInHandler()
        {
            var service = new FileService();

            IFileSession firedSession = null;
            service.Created += delegate(object o, FileSessionEventArgs args)
            {
                firedSession = (IFileSession)o;
            };

            var session = service.OnCreated(this, "*.txt");

            Assert.AreSame(session, firedSession);
        }

        /// <summary>
        /// Thats that OnCreated passes the change origin.
        /// </summary>
        [Test]
        public void OnCreated_Created_HasCorrectOriginInHandler()
        {
            var service = new FileService();
            
            service.Created += delegate(object o, FileSessionEventArgs args)
            {
                Assert.AreSame(this, args.Origin);
            };

            service.OnCreated(this, "*.txt");
        }

        /// <summary>
        /// Tests that OnCreated throws exception when fed with null origin.
        /// </summary>
        [Test]
        [ExpectedException(typeof(ArgumentNullException))]
        public void OnCreated_NullOrigin_ThrowsException()
        {
            var service = new FileService();
            service.OnCreated(null, "*.txt");
        }

        /// <summary>
        /// Tests that OnCreated throws exception when fed with null filename.
        /// </summary>
        [Test]
        [ExpectedException(typeof(ArgumentNullException))]
        public void OnCreated_NullExtension_ThrowsException()
        {
            var service = new FileService();
            service.OnCreated(this, null);
        }

        /// <summary>
        /// Tests that OnCreated throws exception when fed with extension in incorrect format.
        /// </summary>
        [Test]
        [ExpectedException(typeof(ArgumentException))]
        public void OnCreated_MalformedExtension_ThrowsException()
        {
            var service = new FileService();
            service.OnCreated(this, "txt");
        }

        /// <summary>
        /// Thats that OnCreated fires the Created event.
        /// </summary>
        [Test]
        public void OnCreated_Created_FiresCreatedEvent()
        {
            var service = new FileService();

            bool fired = false;
            service.Created += delegate
            {
                fired = true;
            };

            service.OnCreated(this, "*.txt");

            Assert.IsTrue(fired);
        }

        /// <summary>
        /// Thats that OnCreated adds the session to opened sessions.
        /// </summary>
        [Test]
        public void OnCreated_Created_AddsToActiveSessions()
        {
            var service = new FileService();

            var session = service.OnCreated(this, "*.txt");

            Assert.Contains(session, service.ActiveSessions);
        }

        /// <summary>
        /// Thats that OnOpened returns session with the file name passed into it.
        /// </summary>
        [Test]
        public void OnOpened_Always_ReturnsSessionWithThatName()
        {
            var service = new FileService();
            var session = service.OnOpened(this, "ABC");

            Assert.AreEqual("ABC", session.FileName);
        }

        /// <summary>
        /// Tests that OnOpened returns session which is open.
        /// </summary>
        [Test]
        public void OnOpened_Always_ReturnsOpenSession()
        {
            var service = new FileService();

            var result = service.OnOpened(this, "ABC");

            Assert.IsFalse(result.IsClosed);
        }

        /// <summary>
        /// Tests that OnOpened returns session which is not unsaved.
        /// </summary>
        [Test]
        public void OnOpened_Always_ReturnsNotUnsavedSession()
        {
            var service = new FileService();

            var result = service.OnOpened(this, "ABC");

            Assert.IsFalse(result.IsUnsaved);
        }

        /// <summary>
        /// Thats that OnOpened fires the Opened event.
        /// </summary>
        [Test]
        public void OnOpened_Opened_FiresOpenedEvent()
        {
            var service = new FileService();

            bool fired = false;
            service.Opened += delegate
                {
                    fired = true;
                };

            service.OnOpened(this, "ABC");

            Assert.IsTrue(fired);
        }

        /// <summary>
        /// Thats that OnOpened fires the Opened event.
        /// </summary>
        [Test]
        public void OnOpened_Opened_HasCorrectSenderInHandler()
        {
            var service = new FileService();

            IFileSession firedSession = null;
            service.Opened += delegate(object o, FileSessionEventArgs args)
                {
                    firedSession = (IFileSession)o;
                };

            var session = service.OnOpened(this, "ABC");

            Assert.AreSame(session, firedSession);
        }

        /// <summary>
        /// Thats that OnOpened passes the change origin.
        /// </summary>
        [Test]
        public void OnOpened_Opened_HasCorrectOriginInHandler()
        {
            var service = new FileService();

            service.Opened += delegate(object o, FileSessionEventArgs args)
                {
                    Assert.AreSame(this, args.Origin);
                };

            service.OnOpened(this, "ABC");
        }

        /// <summary>
        /// Tests that OnOpened throws exception when fed with null origin.
        /// </summary>
        [Test]
        [ExpectedException(typeof(ArgumentNullException))]
        public void OnOpened_NullOrigin_ThrowsException()
        {
            var service = new FileService();
            service.OnOpened(null, "abc");
        }

        /// <summary>
        /// Tests that OnOpened throws exception when fed with null filename.
        /// </summary>
        [Test]
        [ExpectedException(typeof(ArgumentNullException))]
        public void OnOpened_NullFileName_ThrowsException()
        {
            var service = new FileService();
            service.OnOpened(this, null);
        }

        /// <summary>
        /// Thats that OnOpened adds the session to opened sessions.
        /// </summary>
        [Test]
        public void OnOpened_Opened_AddsToActiveSessions()
        {
            var service = new FileService();

            var session = service.OnOpened(this, "abc");

            Assert.Contains(session, service.ActiveSessions);
        }

        /// <summary>
        /// Thats that OnOpened adds the file to recent files.
        /// </summary>
        [Test]
        public void OnOpened_Opened_AddsToRecentFiles()
        {
            var service = new FileService();

            service.OnOpened(this, "abcd");

            Assert.Contains("abcd", service.RecentFiles);
        }
    }
}
