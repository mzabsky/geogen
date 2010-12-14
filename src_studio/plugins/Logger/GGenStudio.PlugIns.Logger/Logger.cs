﻿using System;
using GeoGen.Studio.PlugInLoader;
using GeoGen.Studio.Utilities.Messaging;
using System.IO;

namespace GeoGen.Studio.PlugIns
{
    public class Logger: IPlugIn
    {
        protected TextWriter Writer {get; set;}

        public Logger()
        {
            try
            {
                this.Writer = new StreamWriter("../log.txt", true);
                this.Writer.WriteLine("===================================================================");
                this.Writer.WriteLine("Starting new instance on " + DateTime.Now);
                this.Writer.Flush();
            }
            catch { };
        }

        public void Register(IMessageProvider messageProvider)
        {
            messageProvider.MessageThrown += this.MessageHandler;
        }

        public void MessageHandler(object sender, MessageThrownEventArgs args)
        {
            try
            {
                this.Writer.WriteLine("[" + sender.GetType().ToString() + " - " + args.Message.DateTime + "] " + args.Message.Type + ": " + args.Message.Text);
                this.Writer.Flush();
            }
            catch {}
        }
    }
}