using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using GeoGen.Net;

namespace GeoGen.Studio
{
    sealed public class ScriptArg: INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        public ScriptArgType Type { get; protected set; }

        public string Name {get; protected set;}

        public string Label { get; protected set; }

        public string Description { get; protected set; }

        public uint Default {get; set;}

        public uint Minimum { get; set; }

        public uint Maximum { get; set; }

        protected uint value;
        public uint Value { 
            get {
                return value;
            }
            set {
                // Clamp the incoming value into supported range.
                 this.value = Math.Min(this.Maximum, Math.Max(this.Minimum, value));
            }
        }

        public string[] Options { get; protected set; }

        public string CurrentOption {
            get {
                return this.Options[this.Value];
            }
        }

        public ScriptArg(){}
    
        public ScriptArg(GeoGen.Net.ScriptArg arg)
        {
            this.Type = arg.Type;
            this.Name = arg.Name;
            this.Label = arg.Label;
            this.Description = arg.Description;
            this.Default = arg.Default;
            this.Maximum = arg.Maximum;
            this.Minimum = arg.Minimum;
            this.Value = arg.Value;

            this.Options = arg.Options.ToArray(); //new string[this.Maximum + 1];
            //arg.Options.CopyTo(this.Options, 0);
        }

        protected void OnPropertyChanged(string info)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(info));
            }
        }
    }
}
