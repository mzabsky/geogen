namespace GeoGen.Studio.UI
{
    using System;
    using System.Windows;
    using System.Windows.Input;
    using System.Windows.Media;
    using System.Windows.Media.Imaging;

    using GeoGen.Studio.Utilities;

    /// <summary>
    /// Interaction logic for MessageBox.xaml
    /// </summary>
    public partial class MessageBox
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="MessageBox"/> class.
        /// </summary>
        /// <param name="buttons">The button mode.</param>
        /// <param name="image">The image.</param>
        private MessageBox(MessageBoxButton buttons, MessageBoxImage image)
        {
            this.ReturnResultCommand = new RelayCommand(
                param =>
                {
                    this.Result = (MessageBoxResult)param;
                    this.Close();
                });

            // CTRL+C will copy the text into system clipboard
            this.CommandBindings.Add(
                new CommandBinding(
                    ApplicationCommands.Copy,
                    delegate
                    {
                        Clipboard.SetText(this.Content.ToString(), TextDataFormat.Text);
                    }));          

            this.Buttons = buttons;
            this.Image = image;

            InitializeComponent();
        }

        /// <summary>
        /// Gets command used by the message box buttons.
        /// </summary>
        /// <value>The return result command.</value>
        public ICommand ReturnResultCommand { get; private set; }

        /// <summary>
        /// Gets the result (instead of insufficient <see cref="Window.DialogResult"/>, which has too few possible values).
        /// </summary>
        public MessageBoxResult Result { get; private set; }

        /// <summary>
        /// Gets the button mode.
        /// </summary>
        /// <value>The button mode.</value>
        public MessageBoxButton Buttons { get; private set; }

        /// <summary>
        /// Gets the icon shown in the dialog.
        /// </summary>
        /// <value>The image.</value>
        public MessageBoxImage Image { get; private set; }

        /// <summary>
        /// Gets the <see cref="ImageSource"/> of the icon.
        /// </summary>
        /// <value>The image source.</value>
        public ImageSource ImageSource
        {
            get
            {
                switch (this.Image)
                {
                    case MessageBoxImage.None:
                        return null;
                    case MessageBoxImage.Hand:
                        return new BitmapImage(new Uri("pack://application:,,,/GeoGen.Studio.Core;component/UI/Images/error.png"));
                    case MessageBoxImage.Question:
                        return new BitmapImage(new Uri("pack://application:,,,/GeoGen.Studio.Core;component/UI/Images/question.png"));
                    case MessageBoxImage.Exclamation:
                        return new BitmapImage(new Uri("pack://application:,,,/GeoGen.Studio.Core;component/UI/Images/warning.png"));
                    case MessageBoxImage.Asterisk:
                        return new BitmapImage(new Uri("pack://application:,,,/GeoGen.Studio.Core;component/UI/Images/message.png"));
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }
        }

        /// <summary>
        /// Displays a message box that contains the specified text, title bar caption, and response buttons.
        /// </summary>
        /// <param name="messageBoxText">The message to display.</param>
        /// <param name="caption">The title of the message box.</param>
        /// <param name="button">A value that indicates the button or buttons to display.</param>
        /// <param name="image">A value that specifies the icon to display.</param>
        /// <returns>A value that indicates the user's response to the message.</returns>
        public static MessageBoxResult Show(
            string messageBoxText,
            string caption = null,
            MessageBoxButton button = MessageBoxButton.OK,
            MessageBoxImage image = MessageBoxImage.Error)
        {
            var messageBox = new MessageBox(button, image)
            {
                Content = messageBoxText,
                Title = caption ?? App.Name
            };

            messageBox.ShowDialog();

            return messageBox.Result;
        }
    }
}
