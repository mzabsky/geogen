using System;
using System.Collections.Generic;
using GeoGen.Studio.Utilities.Collections;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Controls;
using System.Windows.Markup;
using System.Windows;

namespace GeoGen.Studio.PlugIns
{
    [ContentProperty("Items")]
    public class MenuEntry : DependencyObject, IPriority
    {
        private static readonly DependencyProperty HeaderProperty = DependencyProperty.Register(
            "Header", typeof(string), typeof(MenuEntry), new PropertyMetadata("<No header>"));

        public string Header
        {
            get
            {
                return (string)GetValue(HeaderProperty);
            }
            set
            {
                SetValue(HeaderProperty, value);
            }
        }

        private static readonly DependencyProperty PriorityProperty = DependencyProperty.Register(
            "Priority", typeof(double), typeof(MenuEntry), new PropertyMetadata(0d));

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

        private static readonly DependencyProperty CommandProperty = DependencyProperty.Register(
            "Command", typeof(ICommand), typeof(MenuEntry), new PropertyMetadata());

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

        private static readonly DependencyProperty InputGestureTextProperty = DependencyProperty.Register(
            "InputGestureText", typeof(string), typeof(MenuEntry), new PropertyMetadata(""));

        public string InputGestureText
        {
            get
            {
                return (string)GetValue(InputGestureTextProperty);
            }
            set
            {
                SetValue(InputGestureTextProperty, value);
            }
        }

        private static readonly DependencyProperty IconProperty = DependencyProperty.Register(
            "Icon", typeof(ImageSource), typeof(MenuEntry), new PropertyMetadata());

        public ImageSource Icon
        {
            get
            {
                return (ImageSource)GetValue(IconProperty);
            }
            set
            {
                SetValue(IconProperty, value);
            }
        }

        private static readonly DependencyProperty ItemsProperty = DependencyProperty.Register(
            "Items", typeof(PriorityObservableCollection), typeof(MenuEntry), new PropertyMetadata());

        public PriorityObservableCollection Items
        {
            get
            {
                return (PriorityObservableCollection)GetValue(ItemsProperty);
            }
            set
            {
                SetValue(ItemsProperty, value);
            }
        }

        public MenuEntry() { 
            this.Items = new PriorityObservableCollection();
        }

        public MenuEntry(string header, double priority = 0, ICommand command = null, string inputGestureText = null, ImageSource icon = null, PriorityObservableCollection items = null)
        {
            this.Header = header;
            this.Priority = priority;
            this.Items = items ?? new PriorityObservableCollection();
            this.Command = command;
            this.InputGestureText = inputGestureText;
            this.Icon = icon;
        }

        public static implicit operator MenuEntry(MenuItem victim)
        {
            MenuEntry converted = new MenuEntry(victim.Header.ToString(), 0, victim.Command);

            foreach (object child in victim.Items)
            {
                if (child is MenuItem)
                {
                    MenuEntry c = child as MenuItem;
                    converted.Items.Add((MenuEntry)(child as MenuItem));
                }
            }

            return converted;
        }

        internal void MergeWith(MenuEntry menu)
        {
            foreach (MenuEntry item in menu.Items)
            {
                bool isAlreadyInItems = false;

                foreach (MenuEntry ownItem in this.Items)
                {
                    if (ownItem.Header == item.Header)
                    {
                        if (item.Items.Count > 0)
                        {
                            ownItem.MergeWith(item);
                        }

                        isAlreadyInItems = true;
                    }
                }

                if (!isAlreadyInItems)
                {
                    this.Items.Add(item);
                }
            }
        }
    }
}
