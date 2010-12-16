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
using GeoGen.Studio.Utilities;
using GeoGen.Studio.Utilities.Context;

namespace GeoGen.Studio.PlugIns
{
    public class StatusBarEntry : DependencyObject, IPriority
    {
        public double Priority {get; set;}

        protected BindingBase valueBinding = null;
        public BindingBase ValueBinding
        {
            get
            {
                return this.valueBinding;
            }
            set
            {
                BindingOperations.ClearBinding(this, StatusBarEntry.ValueProperty);

                this.valueBinding = value;

                if (value != null)
                {
                    BindingOperations.SetBinding(this, StatusBarEntry.ValueProperty, value);
                }
            }
        }

        public Context Context { get; set; }

        private static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
            "Value", typeof(string), typeof(StatusBarEntry), new PropertyMetadata(null));

        public string Value
        {
            get
            {
                return (string)GetValue(ValueProperty);
            }
            set
            {
                SetValue(ValueProperty, value);
            }
        }

        public static readonly DependencyProperty VisibilityProperty = DependencyProperty.Register(
            "Visibility", typeof(Visibility), typeof(StatusBarEntry), new PropertyMetadata(Visibility.Visible));

        public Visibility Visibility
        {
            get
            {
                return (Visibility)GetValue(VisibilityProperty);
            }
            set
            {
                SetValue(VisibilityProperty, Visibility);
            }
        }

        public StatusBarEntry() {}

        public StatusBarEntry(double priority, string value = "", BindingBase valueBinding = null)
        {
            this.Priority = priority;
            this.Value = value;
            this.ValueBinding = valueBinding;
        }
    }
}
