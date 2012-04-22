namespace GeoGen.Studio.PlugIns.Services
{
    using System;

    using NUnit.Framework;

    // ReSharper disable InconsistentNaming

    /// <summary>
    /// Tests for the <see cref="FileService.FileSession"/> class.
    /// </summary>
    [TestFixture]
    public class FileSessionTests
    {
        /// <summary>
        /// Tests that OnChanged causes the session to become unsaved.
        /// </summary>
        [Test]
        public void OnChanged_Changed_IsUnsavedToTrue()
        {
            var service = new FileService();

            var session = service.OnCreated(this, "*.txt");

            session.OnChanged(this);

            Assert.IsTrue(session.IsUnsaved);
        }

        /// <summary>
        /// Tests that OnChanged fires the changed event.
        /// </summary>
        [Test]
        public void OnChanged_Changed_FiresChangedEvent()
        {
            var service = new FileService();

            var session = service.OnCreated(this, "*.txt");

            bool fired = false;
            session.Changed += delegate { fired = true; };

            session.OnChanged(this);

            Assert.IsTrue(fired);
        }

        /// <summary>
        /// Tests that OnChanged throws exception when fed with null.
        /// </summary>
        [Test]
        [ExpectedException(typeof(ArgumentNullException))]
        public void OnChanged_NullOrigin_ThrowsException()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            session.OnChanged(null);
        }

        /// <summary>
        /// Tests that OnChanged throws exception when the session is already closed.
        /// </summary>
        [Test]
        [ExpectedException(typeof(SessionClosedException))]
        public void OnChanged_ClosedSession_ThrowsException()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            session.OnClosed(this);

            session.OnChanged(this);
        }

        /// <summary>
        /// Thats that OnChanged passes the session as event sender.
        /// </summary>
        [Test]
        public void OnChanged_Changed_HasCorrectSenderInHandler()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            IFileSession firedSession = null;
            session.Changed += delegate(object o, FileSessionEventArgs args)
            {
                firedSession = (IFileSession)o;
            };

            session.OnChanged(this);

            Assert.AreSame(session, firedSession);
        }

        /// <summary>
        /// Thats that OnChanged passes the change origin.
        /// </summary>
        [Test]
        public void OnChanged_Changed_HasCorrectOriginInHandler()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            session.Changed += delegate(object o, FileSessionEventArgs args)
            {
                Assert.AreSame(this, args.Origin);
            };

            session.OnChanged(this);
        }

        /// <summary>
        /// Tests that OnClosed sets Closed as true.
        /// </summary>
        [Test]
        public void OnClosed_Closed_Closes()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            session.OnClosed(this);

            Assert.IsTrue(session.IsClosed);
        }

        /// <summary>
        /// Tests that OnClosed fires the Closed event.
        /// </summary>
        [Test]
        public void OnClosed_Closed_FiresClosedEvent()
        {
            var service = new FileService();

            var session = service.OnCreated(this, "*.txt");

            bool fired = false;
            session.Closed += delegate
            {
                fired = true;
            };

            session.OnClosed(this);

            Assert.IsTrue(fired);
        }

        /// <summary>
        /// Tests that OnClosed throws exception when fed with null.
        /// </summary>
        [Test]
        [ExpectedException(typeof(ArgumentNullException))]
        public void OnClosed_NullOrigin_ThrowsException()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            session.OnClosed(null);
        }

        /// <summary>
        /// Tests that OnClosed throws exception when the session is already closed.
        /// </summary>
        [Test]
        [ExpectedException(typeof(SessionClosedException))]
        public void OnClosed_ClosedSession_ThrowsException()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            session.OnClosed(this);

            session.OnClosed(this);
        }

        /// <summary>
        /// Thats that OnClosed passes the session as event sender.
        /// </summary>
        [Test]
        public void OnClosed_Closed_HasCorrectSenderInHandler()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            IFileSession firedSession = null;
            session.Closed += delegate(object o, FileSessionEventArgs args)
            {
                firedSession = (IFileSession)o;
            };

            session.OnClosed(this);

            Assert.AreSame(session, firedSession);
        }

        /// <summary>
        /// Thats that OnClosed passes the change origin.
        /// </summary>
        [Test]
        public void OnClosed_Closed_HasCorrectOriginInHandler()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            session.Closed += delegate(object o, FileSessionEventArgs args)
            {
                Assert.AreSame(this, args.Origin);
            };

            session.OnClosed(this);
        }

        /// <summary>
        /// Thats that OnClosed removes the session from opened sessions.
        /// </summary>
        [Test]
        public void OnClosed_Closed_RemovesFromActiveSessions()
        {
            var service = new FileService();

            var session = service.OnCreated(this, "*.txt");

            session.OnClosed(this);

            Assert.IsFalse(service.ActiveSessions.Contains(session));
        }

        /// <summary>
        /// Tests that OnSaved sets IsUnsaved as false.
        /// </summary>
        [Test]
        public void OnSaved_Saved_Saves()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");
            session.OnChanged(this);

            session.OnSaved(this);

            Assert.IsFalse(session.IsUnsaved);
        }

        /// <summary>
        /// Tests that OnSaved fires the Saved event.
        /// </summary>
        [Test]
        public void OnSaved_Saved_FiresSavedEvent()
        {
            var service = new FileService();

            var session = service.OnCreated(this, "*.txt");

            bool fired = false;
            session.Saved += delegate
            {
                fired = true;
            };

            session.OnSaved(this);

            Assert.IsTrue(fired);
        }

        /// <summary>
        /// Tests that OnSaved throws exception when fed with null.
        /// </summary>
        [Test]
        [ExpectedException(typeof(ArgumentNullException))]
        public void OnSaved_NullOrigin_ThrowsException()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            session.OnSaved(null);
        }

        /// <summary>
        /// Tests that OnSaved throws exception when the session is already closed.
        /// </summary>
        [Test]
        [ExpectedException(typeof(SessionClosedException))]
        public void OnSaved_ClosedSession_ThrowsException()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            session.OnClosed(this);

            session.OnSaved(this);
        }

        /// <summary>
        /// Thats that OnSaved passes the session as event sender.
        /// </summary>
        [Test]
        public void OnSaved_Saved_HasCorrectSenderInHandler()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            IFileSession firedSession = null;
            session.Saved += delegate(object o, FileSessionEventArgs args)
            {
                firedSession = (IFileSession)o;
            };

            session.OnSaved(this);

            Assert.AreSame(session, firedSession);
        }

        /// <summary>
        /// Thats that OnSaved passes the change origin.
        /// </summary>
        [Test]
        public void OnSaved_Saved_HasCorrectOriginInHandler()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            session.Saved += delegate(object o, FileSessionEventArgs args) { Assert.AreSame(this, args.Origin); };

            session.OnSaved(this);
        }

        /// <summary>
        /// Thats that OnSavedAs returns session with correct name.
        /// </summary>
        [Test]
        public void OnSavedAs_Saved_ReturnsSessionHasCorrectFileName()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            var newSession = session.OnSavedAs(this, "abc");

            Assert.AreEqual("abc", newSession.FileName);
        }

        /// <summary>
        /// Thats that OnSavedAs returns another session.
        /// </summary>
        [Test]
        public void OnSavedAs_Saved_ReturnsAnotherInstance()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            var newSession = session.OnSavedAs(this, "abc");

            Assert.AreNotSame(session, newSession);
        }

        /// <summary>
        /// Thats that OnSavedAs closes the original session.
        /// </summary>
        [Test]
        public void OnSavedAs_Saved_ClosesItself()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            session.OnSavedAs(this, "abc");

            Assert.IsTrue(session.IsClosed);
        }

        /// <summary>
        /// Thats that OnSavedAs returns open session.
        /// </summary>
        [Test]
        public void OnSavedAs_Saved_ReturnsOpenSession()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            var newSession = session.OnSavedAs(this, "abc");

            Assert.IsFalse(newSession.IsClosed);
        }

        /// <summary>
        /// Thats that OnSavedAs returns saved session.
        /// </summary>
        [Test]
        public void OnSavedAs_Saved_ReturnsSavedSession()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            var newSession = session.OnSavedAs(this, "abc");

            Assert.IsFalse(newSession.IsUnsaved);
        }

        /// <summary>
        /// Tests that OnSavedAs fires Closed event on the original session.
        /// </summary>
        [Test]
        public void OnSavedAs_Saved_FiresClosedEvent()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            bool fired = false;
            session.Closed += delegate(object o, FileSessionEventArgs args)
            {
                if (o == session)
                {
                    fired = true;
                }
            };

            session.OnSavedAs(this, "abc");

            Assert.IsTrue(fired);
        }

        /// <summary>
        /// Tests that OnSavedAs fires Opened event.
        /// </summary>
        [Test]
        public void OnSavedAs_Saved_FiresOpenedEvent()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            bool fired = false;
            service.Opened += delegate { fired = true; };

            session.OnSavedAs(this, "abc");

            Assert.IsTrue(fired);
        }

        /// <summary>
        /// Tests that the Opened evend fired by OnSavedAs is fired on the instance which is later returned.
        /// </summary>
        [Test]
        public void OnSavedAs_Saved_FiresOpenedEventOnReturnedInstance()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            IFileSession firedInstance = null;
            service.Opened += delegate(object o, FileSessionEventArgs args) { firedInstance = o as IFileSession; };

            var returnedInstance = session.OnSavedAs(this, "abc");

            Assert.AreSame(firedInstance, returnedInstance);
        }

        /// <summary>
        /// Tests that the Saved evend fired by OnSavedAs is fired on the instance which is later returned.
        /// </summary>
        [Test]
        public void OnSavedAs_Saved_FiresSavedOnReturnedInstance()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            bool fired = false;
            service.Opened +=
                delegate(object o, FileSessionEventArgs args)
                {
                    var newSession = (IFileSession)o;
                    newSession.Saved += delegate(object o2, FileSessionEventArgs args2)
                    {
                        if (o2 == newSession)
                        {
                            fired = true;
                        }
                    };
                };

            session.OnSavedAs(this, "abc");

            Assert.IsTrue(fired);
        }

        /// <summary>
        /// Tests that OnSavedAs throws exception when fed with null origin.
        /// </summary>
        [Test]
        [ExpectedException(typeof(ArgumentNullException))]
        public void OnSavedAs_NullOrigin_ThrowsException()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            session.OnSavedAs(null, "abc");
        }

        /// <summary>
        /// Tests that OnSavedAs throws exception when fed with null filename.
        /// </summary>
        [Test]
        [ExpectedException(typeof(ArgumentNullException))]
        public void OnSavedAs_NullFileName_ThrowsException()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            session.OnSavedAs(this, null);
        }

        /// <summary>
        /// Tests that OnSavedAs throws exception when the session is already closed.
        /// </summary>
        [Test]
        [ExpectedException(typeof(SessionClosedException))]
        public void OnSavedAs_ClosedSession_ThrowsException()
        {
            var service = new FileService();
            var session = service.OnCreated(this, "*.txt");

            session.OnClosed(this);

            session.OnSavedAs(this, "abc");
        }

        /// <summary>
        /// Thats that OnSavedAs removes the original session from opened sessions.
        /// </summary>
        [Test]
        public void OnSavedAs_Saved_RemovesOriginalFromActiveSessions()
        {
            var service = new FileService();

            var session = service.OnCreated(this, "*.txt");

            session.OnSavedAs(this, "abc");

            Assert.IsFalse(service.ActiveSessions.Contains(session));
        }

        /// <summary>
        /// Thats that OnSavedAs adds the new session to opened sessions.
        /// </summary>
        [Test]
        public void OnSavedAs_Saved_AddsNewToActiveSessions()
        {
            var service = new FileService();

            var session = service.OnCreated(this, "*.txt");

            var newSession = session.OnSavedAs(this, "abc");

            Assert.Contains(newSession, service.ActiveSessions);
        }

        /// <summary>
        /// Thats that OnSavedAs adds the file to recent files.
        /// </summary>
        [Test]
        public void OnOpened_Opened_AddsToRecentFiles()
        {
            var service = new FileService();

            var session = service.OnCreated(this, "*.txt");

            session.OnSavedAs(this, "abcd");

            Assert.Contains("abcd", service.RecentFiles);
        }

        /// <summary>
        /// Tests that Extension returns correct extension.
        /// </summary>
        [Test]
        public void Extension_TrickyFileName_ReturnsCorrectExtension()
        {
            var service = new FileService();

            var session = service.OnOpened(this, "Abc.def.GHI.extEnsio_n");

            Assert.AreEqual(".extEnsio_n", session.Extension);
        }

        /// <summary>
        /// Tests that Extension returns empty string if there is no dot in the file name.
        /// </summary>
        [Test]
        public void Extension_NoDotInFileName_ReturnsCorrectExtension()
        {
            var service = new FileService();

            var session = service.OnOpened(this, "noExtension");

            Assert.AreEqual(string.Empty, session.Extension);
        }
    }
}
