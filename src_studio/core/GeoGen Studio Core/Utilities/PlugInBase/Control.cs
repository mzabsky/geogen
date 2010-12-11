using System.Windows;
using System.Windows.Controls;
using System.ComponentModel;
using GeoGen.Studio.Utilities.Messaging;
using GeoGen.Studio.PlugInLoader;

namespace GeoGen.Studio.Utilities.PlugInBase
{
    public class Control : UserControl, IPlugIn, IMessageProvider, INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        public event MessageThrownEventHandler MessageThrown;

        protected void ThrowMessage(Message message)
        {
            MessageThrown(this, new MessageThrownEventArgs(message));
        }

        protected void NotifyPropertyChanged(string info)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(info));
            }
        }
    }
}
