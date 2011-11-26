using System;
using System.Collections.Generic;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Controls;
using System.Windows.Markup;
using System.Windows;
using GeoGen.Studio.Utilities.Collections;
using System.Windows.Data;
using System.Windows.Media.Imaging;
using System.Windows.Controls.Primitives;

namespace GeoGen.Studio.PlugIns
{
    abstract public class ToolBarEntry : UserControl, IPriority
    {
        private static readonly DependencyProperty PriorityProperty = DependencyProperty.Register(
            "Priority", typeof(double), typeof(ToolBarEntry), new PropertyMetadata(0d));

        public double Priority
        {
            get
            {
                return (double)GetValue(PriorityProperty);
            }
            set
            {
                SetValue(PriorityProperty, value);
            }
        }

        private static readonly DependencyProperty ToolTipProperty = DependencyProperty.Register(
            "ToolTip", typeof(object), typeof(ToolBarEntry), new PropertyMetadata());

        public object ToolTip
        {
            get
            {
                return (object)GetValue(ToolTipProperty);
            }
            set
            {
                SetValue(ToolTipProperty, value);
            }
        }

        private static readonly DependencyProperty DataContextProperty = DependencyProperty.Register(
            "DataContext", typeof(object), typeof(ToolBarEntry), new PropertyMetadata(null));

        public object DataContext
        {
            get
            {
                return (object)GetValue(DataContextProperty);
            }
            set
            {
                SetValue(DataContextProperty, value);
            }
        }

        public static implicit operator ToolBarEntry(Button victim)
        {
            ToolBarEntry converted = new ToolBarButton(
                icon: victim.Content,
                toolTip: victim.ToolTip,
                command: victim.Command
            );

            return converted;
        }

        public static implicit operator ToolBarEntry(ToggleButton victim)
        {
            ToolBarEntry converted = new ToolBarCheckableButton(
                icon: victim.Content,
                toolTip: victim.ToolTip,
                command: victim.Command,
                isChecked: (bool) victim.IsChecked
            );

            return converted;
        }

        public static implicit operator ToolBarEntry(Separator victim)
        {
            ToolBarEntry converted = new ToolBarSeparator();
            return converted;
        }
    }
}
