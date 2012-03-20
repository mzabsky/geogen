namespace GeoGen.Studio.PlugIns
{
    using System;
    using System.IO;

    using GeoGen.Studio.PlugInLoader;
    using GeoGen.Studio.Utilities.Messaging;

    /// <summary>
    /// Writes messages thrown by other components into a log file.
    /// </summary>
    [PlugIn(VisibleInList = false)]
    public class Logger : IPlugIn
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="Logger"/> class.
        /// </summary>
        public Logger()
        {
            try
            {
                this.Writer = new StreamWriter("../log.txt", true);
                this.Writer.WriteLine("===================================================================");
                this.Writer.WriteLine("Starting new instance on " + DateTime.Now);
                this.Writer.Flush();
            }
            catch (IOException)
            {
                new Message("Could not open log file.", MessageType.Error);
            }
        }

        /// <summary>
        /// Gets or sets the write stream.
        /// </summary>
        /// <value>
        /// The writer.
        /// </value>
        protected TextWriter Writer { get; set; }

        /// <summary>
        /// Registers this instance as a plug-in.
        /// </summary>
        public void Register()
        {
            foreach (var message in Messenger.Instance.MessageHistory)
            {
                this.HandleMessage(null, new MessageThrownEventArgs(message));
            }

            Messenger.Instance.MessageSent += this.HandleMessage;            
        }

        /// <summary>
        /// Handles the message.
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="args">The <see cref="GeoGen.Studio.Utilities.Messaging.MessageThrownEventArgs"/> instance containing the event data.</param>
        protected void HandleMessage(object sender, MessageThrownEventArgs args)
        {
            if (this.Writer == null)
            {
                return;
            }

            this.Writer.WriteLine("[" + args.Message.DateTime + "] " + args.Message.Type + ": " + args.Message.Text);
            this.Writer.Flush();
        }
    }
}
