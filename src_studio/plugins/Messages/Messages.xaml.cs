using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using GeoGen.Studio.Utilities.Messaging;
using System.Collections.ObjectModel;
using GeoGen.Studio.PlugInLoader;

namespace GeoGen.Studio.PlugIns
{
    /// <summary>
    /// Interaction logic for UserControl1.xaml
    /// </summary>
    public partial class Messages : UserControl, IPlugIn
    {
        public ObservableCollection<Message> Items { get; private set; }

        private static readonly DependencyPropertyKey IsEmptyPropertyKey = DependencyProperty.RegisterReadOnly(
            "IsEmpty", typeof(bool), typeof(Messages), new PropertyMetadata(true));

        public static readonly DependencyProperty IsEmptyProperty = IsEmptyPropertyKey.DependencyProperty;

        public bool IsEmpty
        {
            get {
                return (bool) GetValue(IsEmptyProperty); 
            }
            private set {
                SetValue(IsEmptyPropertyKey, value); 
            }
        }

        public Messages()
        {
            this.Items = new ObservableCollection<Message>();
            this.Items.CollectionChanged += this.CollectionChanged;

            InitializeComponent();           
        }

        public void Register(IDockManager dockManager)
        {
            dockManager.AddAsDockableContent(this, "Messages", false);
        }

        public void Register(IMessageProvider messageProvider)
        {
            messageProvider.MessageThrown += delegate(object o, MessageThrownEventArgs args)
            {
                this.Items.Add(args.Message);
            };            
        }

        private void CollectionChanged(object sender, EventArgs args)
        {
            this.IsEmpty = (this.Items.Count == 0);

            if(this.scroller.ScrollableHeight == this.scroller.VerticalOffset) this.scroller.ScrollToBottom();
        }
		
		private void CopyToClipboard(object sender, RoutedEventArgs e){
            Clipboard.SetText(((Message)((MenuItem)sender).DataContext).Text);
		}

        private void Delete(object sender, RoutedEventArgs e)
        {
            this.Items.Remove((Message)((MenuItem)sender).DataContext);
        }

        private void Clear(object sender, RoutedEventArgs e)
        {
            this.Items.Clear();
        }

        public void AddMessage(string message, MessageType type = MessageType.Message)
        {
            this.Items.Add(new Message(message, type));
        }
	}
}