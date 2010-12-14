using System;
using System.Windows;
using System.Windows.Input;
using System.Windows.Controls;
using System.Windows.Media.Imaging;
using System.Windows.Data;

namespace GeoGen.Studio.PlugIns
{
    public class ToolBarCheckableButton: ToolBarButton
    {
        private static readonly DependencyProperty IsCheckedBindingProperty = DependencyProperty.Register(
            "IsCheckedBinding", typeof(Binding), typeof(ToolBarCheckableButton), new PropertyMetadata(null));

        public Binding IsCheckedBinding
        {
            get
            {
                return (Binding)GetValue(IsCheckedBindingProperty);
            }
            set
            {
                SetValue(IsCheckedBindingProperty, value);
            }
        }

        private static readonly DependencyProperty IsCheckedProperty = DependencyProperty.Register(
            "IsChecked", typeof(bool), typeof(ToolBarCheckableButton), new PropertyMetadata(null));

        public bool IsChecked
        {
            get
            {
                return (bool)GetValue(IsCheckedProperty);
            }
            set
            {
                SetValue(IsCheckedProperty, value);
            }
        }

        public ToolBarCheckableButton(){}

        public ToolBarCheckableButton(object icon, double priority = 0, ICommand command = null, object toolTip = null, bool isChecked = false, Binding isCheckedBinding = null, object dataContext = null)
        {
            //this.Icon = icon;
            if(icon is string)
            {
                Image iconImage = new Image();
                iconImage.Source = new BitmapImage(new Uri(icon as string));
                iconImage.Width = 16;
                iconImage.Height = 16;                    
                    
                this.Icon = iconImage;
            }
            else 
            {
                this.Icon = icon;
            }

            this.IsChecked = isChecked;
            this.IsCheckedBinding = isCheckedBinding;
            this.Priority = priority;
            this.Command = command;
            this.ToolTip = toolTip;
            this.DataContext = dataContext;

            if (IsCheckedBinding != null)
            {
                if (isCheckedBinding.Source == null)
                {
                    isCheckedBinding.Source = this.DataContext;
                }

                BindingOperations.SetBinding(this, ToolBarCheckableButton.IsCheckedProperty, this.IsCheckedBinding);
            }
        }
    }
}
