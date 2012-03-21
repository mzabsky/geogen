namespace GeoGen.Studio.PlugIns
{
    using System;
    using System.Collections.ObjectModel;
    using System.Windows;
    using System.Windows.Controls;

    using GeoGen.Studio.PlugInLoader;
    using GeoGen.Studio.Utilities.Messaging;
    using GeoGen.Studio.Utilities.Persistence;

    /// <summary>
    /// Sub-window which displays list of messages sent by other components.
    /// </summary>
    public partial class Messages : UserControl, IPlugIn
    {
        /// <summary>
        /// <see cref="IsEmpty"/> dependency property.
        /// </summary>
        private static readonly DependencyPropertyKey IsEmptyPropertyKey = DependencyProperty.RegisterReadOnly(
            "IsEmpty", typeof(bool), typeof(Messages), new PropertyMetadata(true));

        /// <summary>
        /// <see cref="IsEmpty"/> dependency property key.
        /// </summary>
        private static readonly DependencyProperty IsEmptyProperty = IsEmptyPropertyKey.DependencyProperty;        

        /// <summary>
        /// Initializes a new instance of the <see cref="Messages"/> class.
        /// </summary>
        public Messages()
        {
            this.Items = new ObservableCollection<Message>();
            this.Items.CollectionChanged += this.CollectionChanged;

            foreach (var message in Messenger.Instance.MessageHistory)
            {
                this.Items.Add(message);
            }

            Messenger.Instance.MessageSent +=
                delegate(object o, MessageThrownEventArgs args) { this.Items.Add(args.Message); };

            InitializeComponent();
            MainConfig.Register(this);

            this.scroller.ContextMenuOpening +=
                delegate(object o, ContextMenuEventArgs args) { args.Handled = this.IsEmpty; };
        }

        /// <summary>
        /// Gets the collection of items currently displayed in the control.
        /// </summary>
        public ObservableCollection<Message> Items { get; private set; }

        /// <summary>
        /// Gets or sets the state of the docking.
        /// </summary>
        /// <value>
        /// The state of the docking.
        /// </value>
        [Persistent(DefaultValue = DockingLocation.RightTop)]
        public object DockingState { get; set; }

        /// <summary>
        /// Gets a value indicating whether there are no messages to be displayed.
        /// </summary>
        /// <value>
        ///   <c>true</c> if there are no messages; otherwise, <c>false</c>.
        /// </value>
        public bool IsEmpty
        {
            get
            {
                return (bool)GetValue(IsEmptyProperty);
            }

            private set
            {
                SetValue(IsEmptyPropertyKey, value);
            }
        }

        /// <summary>
        /// Registers this plug-in with the docking manager.
        /// </summary>
        /// <param name="dockManager">The dock manager.</param>
        public void Register(IDockManager dockManager)
        {
            dockManager.AddAsDockableContent(this, "Messages", DockingLocation.RightBottom);
        }

        /// <summary>
        /// Adds a message to the displayed list. However primary mean to display messages is <see cref="Message.Send"/>.
        /// </summary>
        /// <param name="message">The message.</param>
        /// <param name="type">The type.</param>
        public void AddMessage(string message, MessageType type = MessageType.Message)
        {
            this.Items.Add(new Message(message, type));
        }

        /// <summary>
        /// Handles the <see cref="CollectionChanged"/> event of the <see cref="Items"/> collection.
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="args">The <see cref="System.EventArgs"/> instance containing the event data.</param>
        private void CollectionChanged(object sender, EventArgs args)
        {
            this.IsEmpty = this.Items.Count == 0;

            if (this.scroller != null)
            {
                if (this.scroller.ScrollableHeight == this.scroller.VerticalOffset)
                {
                    this.scroller.ScrollToBottom();
                }
            }
        }

        /// <summary>
        /// Copies an item to clipboard.
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="e">The <see cref="System.Windows.RoutedEventArgs"/> instance containing the event data.</param>
        private void CopyToClipboard(object sender, RoutedEventArgs e)
        {
            Clipboard.SetText(((Message)((MenuItem)sender).DataContext).Text);
        }

        /// <summary>
        /// Deletes an item from the list.
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="e">The <see cref="System.Windows.RoutedEventArgs"/> instance containing the event data.</param>
        private void Delete(object sender, RoutedEventArgs e)
        {
            this.Items.Remove((Message)((MenuItem)sender).DataContext);
        }

        /// <summary>
        /// Removes all items from the list.
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="e">The <see cref="System.Windows.RoutedEventArgs"/> instance containing the event data.</param>
        private void Clear(object sender, RoutedEventArgs e)
        {
            this.Items.Clear();
        }
    }
}