using System;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using GeoGen.Studio.Utilities;

namespace GeoGen.Studio.UI
{
    /// <summary>
    /// Interaction logic for MessageBox.xaml
    /// </summary>
    public partial class MessageBox
    {
        private readonly ICommand returnResultCommand;

        /// <summary>
        /// Command used by the message box buttons.
        /// </summary>
        /// <value>The return result command.</value>
        public ICommand ReturnResultCommand 
        {
            get
            {
                return this.returnResultCommand;
            } 
        }

        /// <summary>
        /// Button mode.
        /// </summary>
        /// <value>The button mode.</value>
        public MessageBoxButton Buttons { get; protected set; }

        /// <summary>
        /// Icon shown in the dialog.
        /// </summary>
        /// <value>The image.</value>
        public MessageBoxImage Image { get; protected set; }

        /// <summary>
        /// <see cref="ImageSource"/> of the icon.
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
        /// Initializes a new instance of the <see cref="MessageBox"/> class.
        /// </summary>
        protected MessageBox(MessageBoxButton buttons, MessageBoxImage image)
        {
            this.returnResultCommand = new RelayCommand(
                param =>
                {
                    this.DialogResult = (bool?) param;
                    this.Close();
                }
            );

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
            MessageBoxImage image = MessageBoxImage.Error
        )
        {
            MessageBox messageBox = new MessageBox(button, image)
            {
                Content = messageBoxText,
                Title = caption ?? App.Name
            };
            bool? result = messageBox.ShowDialog();
            switch (button)
            {
                case MessageBoxButton.OK:
                    return MessageBoxResult.OK;
                case MessageBoxButton.OKCancel:
                    return result == true ? MessageBoxResult.OK : MessageBoxResult.Cancel;
                case MessageBoxButton.YesNoCancel:
                    if (result == true) return MessageBoxResult.Yes;
                    if (result == false) return MessageBoxResult.No;
                    return MessageBoxResult.Cancel;
                case MessageBoxButton.YesNo:
                    return result == true ? MessageBoxResult.Yes : MessageBoxResult.No;
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }
    }
}
