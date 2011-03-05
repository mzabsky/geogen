using System;
using System.Windows;
using System.Windows.Controls;
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

            foreach(Message message in Messenger.MessageHistory)
            {
                this.Items.Add(message);
            }

            Messenger.MessageThrown += delegate(object o, MessageThrownEventArgs args)
            {
                this.Items.Add(args.Message);
            };     

            InitializeComponent();
        }

        public void Register(IDockManager dockManager)
        {
            dockManager.AddAsDockableContent(this, "Messages", false);
        }

        private void CollectionChanged(object sender, EventArgs args)
        {
            this.IsEmpty = (this.Items.Count == 0);

            if (this.scroller != null)
            {
                if (this.scroller.ScrollableHeight == this.scroller.VerticalOffset) this.scroller.ScrollToBottom();
            }
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