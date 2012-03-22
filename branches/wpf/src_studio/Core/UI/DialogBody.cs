namespace GeoGen.Studio.UI
{
    using System.Collections;
    using System.Windows;
    using System.Windows.Controls;

    /// <summary>
    /// Provides united layout for any dialog-type window.
    /// </summary>
    public class DialogBody : ContentControl
    {
        /// <summary>
        /// Dependecy property for <see cref="Buttons"/>.
        /// </summary>
        private static readonly DependencyProperty ButtonsProperty = DependencyProperty.Register(
            "Buttons", typeof(IList), typeof(DialogBody), new PropertyMetadata(null));

        /// <summary>
        /// Dependecy property for <see cref="BottomBarContent"/>.
        /// </summary>
        private static readonly DependencyProperty BottomBarContentProperty = DependencyProperty.Register(
            "BottomBarContent", typeof(object), typeof(DialogBody), new PropertyMetadata(null));

        /// <summary>
        /// Initializes a new instance of the <see cref="DialogBody"/> class.
        /// </summary>
        public DialogBody()
        {
            this.Buttons = new ArrayList();
        }

        /// <summary>
        /// Gets or sets collection of buttons displayed in the bottom bar.
        /// </summary>
        /// <value>The buttons.</value>
        public IList Buttons
        {
            get
            {
                return (IList)GetValue(ButtonsProperty);
            }

            set
            {
                SetValue(ButtonsProperty, value);
            }
        }        

        /// <summary>
        /// Gets or sets content to be displayed in the bottom part of the bottom bar.
        /// </summary>
        /// <value>The content.</value>
        public object BottomBarContent
        {
            get
            {
                return GetValue(BottomBarContentProperty);
            }

            set
            {
                SetValue(BottomBarContentProperty, value);
            }
        }
    }
}
