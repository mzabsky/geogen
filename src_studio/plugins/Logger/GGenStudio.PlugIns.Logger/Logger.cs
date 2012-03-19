namespace GeoGen.Studio.PlugIns
{
    using System;
    using GeoGen.Studio.PlugInLoader;
    using GeoGen.Studio.Utilities.Messaging;
    using System.IO;

    [PlugIn(VisibleInList = false)]
    public class Logger: IPlugIn
    {
        protected TextWriter Writer { get; set; }

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

        public void Register()
        {
            foreach (var message in Messenger.Instance.MessageHistory)
            {
                this.MessageHandler(null, new MessageThrownEventArgs(message));
            }

            Messenger.Instance.MessageSent += this.MessageHandler;            
        }

        public void MessageHandler(object sender, MessageThrownEventArgs args)
        {
            if (this.Writer != null)
            {
                this.Writer.WriteLine("[" + args.Message.DateTime + "] " + args.Message.Type + ": " + args.Message.Text);
                this.Writer.Flush();
            }
        }
    }
}
