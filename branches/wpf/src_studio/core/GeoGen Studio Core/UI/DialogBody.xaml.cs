using System.Collections;
using System.Windows;
using System.Collections.ObjectModel;

namespace GeoGen.Studio.UI
{
    /// <summary>
    /// Provides united layout for any dialog-type window.
    /// </summary>
    public partial class DialogBody
    {
        private static readonly DependencyProperty buttonsProperty = DependencyProperty.Register(
            "Buttons", typeof(IList), typeof(DialogBody), new PropertyMetadata(null));

        /// <summary>
        /// Collection of buttons displayed in the bottom bar.
        /// </summary>
        /// <value>The buttons.</value>
        public IList Buttons
        {
            get
            {
                return (IList)GetValue(buttonsProperty);
            }
            set
            {
                SetValue(buttonsProperty, value);
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="DialogBody"/> class.
        /// </summary>
        public DialogBody()
        {
            this.Buttons = new ArrayList();
            InitializeComponent();
        }
    }
}
