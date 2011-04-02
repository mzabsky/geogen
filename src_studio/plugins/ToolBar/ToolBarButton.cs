using System;
using System.Windows;
using System.Windows.Input;
using System.Windows.Controls;
using System.Windows.Media.Imaging;

namespace GeoGen.Studio.PlugIns
{
    public class ToolBarButton: ToolBarEntry
    {
        private static readonly DependencyProperty CommandProperty = DependencyProperty.Register(
            "Command", typeof(ICommand), typeof(ToolBarButton), new PropertyMetadata());

        public ICommand Command
        {
            get
            {
                return (ICommand)GetValue(CommandProperty);
            }
            set
            {
                SetValue(CommandProperty, value);
            }
        }

        private static readonly DependencyProperty IconProperty = DependencyProperty.Register(
            "Icon", typeof(object), typeof(ToolBarEntry), new PropertyMetadata());

        public object Icon
        {
            get
            {
                return (object)GetValue(IconProperty);
            }
            set
            {
                SetValue(IconProperty, value);
            }
        }

        static ToolBarButton()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(ToolBarButton),
                new FrameworkPropertyMetadata(typeof(ToolBarButton)));            
        }

        public ToolBarButton(){}

        public ToolBarButton(object icon, double priority = 0, ICommand command = null, object toolTip = null)
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

            this.Priority = priority;
            this.Command = command;
            this.ToolTip = toolTip;
        }
    }
}
