using System;
using System.Windows;
using System.Windows.Controls;

namespace GeoGen.Studio.UI
{
    /// <summary>
    /// Interaction logic for NumericBox.xaml
    /// </summary>
    public partial class NumericBox : UserControl
    {
        public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
            "Value", typeof(int), typeof(NumericBox), new FrameworkPropertyMetadata(0, new PropertyChangedCallback(OnValueChanged))
        );

        protected static void OnValueChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            NumericBox numericBox = (NumericBox) d;
            
            if(numericBox.Value > numericBox.Maximum || numericBox.Value < numericBox.Minimum)
            {
                numericBox.Value = Math.Min(Math.Max(numericBox.Value, numericBox.Minimum), numericBox.Maximum);
            }
        }

        /// <summary>
        /// Current value selected in the control.
        /// </summary>
        public int Value
        {
            get
            {
                return (int)GetValue(ValueProperty);
            }
            set
            {
                SetValue(ValueProperty, value);
            }
        }

        private static readonly DependencyProperty MinimumProperty = DependencyProperty.Register(
            "Minimum", typeof(int), typeof(NumericBox), new FrameworkPropertyMetadata(int.MinValue)
        );

        /// <summary>
        /// Current Minimum selected in the control.
        /// </summary>
        public int Minimum
        {
            get
            {
                return (int)GetValue(MinimumProperty);
            }
            set
            {
                SetValue(MinimumProperty, value);
            }
        }

        private static readonly DependencyProperty MaximumProperty = DependencyProperty.Register(
            "Maximum", typeof(int), typeof(NumericBox), new FrameworkPropertyMetadata(int.MaxValue)
        );

        /// <summary>
        /// Current Maximum selected in the control.
        /// </summary>
        public int Maximum
        {
            get
            {
                return (int)GetValue(MaximumProperty);
            }
            set
            {
                SetValue(MaximumProperty, value);
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="NumericBox"/> class.
        /// </summary>
        public NumericBox()
        {
            InitializeComponent();
        }
    }
}
