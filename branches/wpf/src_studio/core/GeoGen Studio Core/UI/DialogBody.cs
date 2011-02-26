using System.Collections;
using System.Windows;
using System.Collections.ObjectModel;
using System.Windows.Controls;

namespace GeoGen.Studio.UI
{
    /// <summary>
    /// Provides united layout for any dialog-type window.
    /// </summary>
    public class DialogBody: UserControl
    {
        public static readonly DependencyProperty ButtonsProperty = DependencyProperty.Register(
            "Buttons", typeof(IList), typeof(DialogBody), new PropertyMetadata(null));

        /// <summary>
        /// Collection of buttons displayed in the bottom bar.
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
        /// Initializes a new instance of the <see cref="DialogBody"/> class.
        /// </summary>
        public DialogBody()
        {
            this.Buttons = new ArrayList();
        }
    }
}
