namespace GeoGen.Studio
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Linq;
    using GeoGen.Net;
    using GeoGen.Studio.Utilities;

    sealed public class ScriptArg: ObservableObject, INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        public ScriptArgType Type { get; private set; }

        public string Name {get; private set;}

        public string Label { get; private set; }

        public string Description { get; private set; }

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
                 this.value = Math.Min(this.Maximum, Math.Max(1, value));
            }
        }

        public string[] Options { get; private set; }

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
