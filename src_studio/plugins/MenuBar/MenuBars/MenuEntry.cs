namespace GeoGen.Studio.PlugIns.MenuBars
{
    using System;
    using System.ComponentModel;
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Data;
    using System.Windows.Input;
    using System.Windows.Markup;
    using System.Windows.Media.Imaging;

    using GeoGen.Studio.Utilities.Collections;

    [ContentProperty("Items")]
    public class MenuEntry : DependencyObject, IPriority, INotifyPropertyChanged
    {
        #region Constants and Fields

        private static readonly DependencyProperty CommandProperty = DependencyProperty.Register(
            "Command", typeof(ICommand), typeof(MenuEntry), new PropertyMetadata());

        private static readonly DependencyProperty DataContextProperty = DependencyProperty.Register(
            "DataContext", typeof(object), typeof(MenuEntry), new PropertyMetadata(null));

        private static readonly DependencyProperty HeaderProperty = DependencyProperty.Register(
            "Header", typeof(string), typeof(MenuEntry), new PropertyMetadata("<No header>"));

        private static readonly DependencyProperty IconProperty = DependencyProperty.Register(
            "Icon", typeof(object), typeof(MenuEntry), new PropertyMetadata());

        private static readonly DependencyProperty InputGestureTextProperty = DependencyProperty.Register(
            "InputGestureText", typeof(string), typeof(MenuEntry), new PropertyMetadata(string.Empty));

        private static readonly DependencyProperty IsCheckableProperty = DependencyProperty.Register(
            "IsCheckable", typeof(bool), typeof(MenuEntry), new PropertyMetadata(null));

        private static readonly DependencyProperty IsCheckedBindingProperty = DependencyProperty.Register(
            "IsCheckedBinding", typeof(Binding), typeof(MenuEntry), new PropertyMetadata(null));

        private static readonly DependencyProperty IsCheckedProperty = DependencyProperty.Register(
            "IsChecked", typeof(bool), typeof(MenuEntry), new PropertyMetadata(null));

        private static readonly DependencyProperty ItemsProperty = DependencyProperty.Register(
            "Items", typeof(MenuEntryObservableCollection), typeof(MenuEntry), new PropertyMetadata());

        #endregion

        #region Constructors and Destructors

        public MenuEntry()
        {
            this.Items = new MenuEntryObservableCollection();
        }

        public MenuEntry(
            string header, 
            double priority = 0, 
            ICommand command = null, 
            string inputGestureText = null, 
            bool isCheckable = false, 
            bool isChecked = false, 
            Binding isCheckedBinding = null, 
            object dataContext = null, 
            object icon = null, 
            MenuEntryObservableCollection items = null)
        {
            this.Header = header;
            this.Priority = priority;
            this.Items = items ?? new MenuEntryObservableCollection();
            this.Command = command;
            this.InputGestureText = inputGestureText;
            this.IsCheckable = isCheckable;
            this.IsChecked = isChecked;
            this.IsCheckedBinding = isCheckedBinding;
            this.DataContext = dataContext;
            
            if (icon != null)
            {
                if (icon is string)
                {
                    var iconImage = new Image
                    {
                        Source = new BitmapImage(new Uri(icon as string)), 
                        Width = 16, 
                        Height = 16
                    };

                    this.Icon = iconImage;
                }
                else
                {
                    this.Icon = icon;
                }
            }

            if (this.IsCheckedBinding != null)
            {
                if (this.IsCheckedBinding.Source == null)
                {
                    this.IsCheckedBinding.Source = dataContext;
                }

                BindingOperations.SetBinding(this, MenuEntry.IsCheckedProperty, this.IsCheckedBinding);
            }
        }

        #endregion

        #region Public Events

        public event PropertyChangedEventHandler PropertyChanged;

        #endregion

        #region Public Properties

        public ICommand Command
        {
            get
            {
                return (ICommand)this.GetValue(CommandProperty);
            }

            set
            {
                this.SetValue(CommandProperty, value);
            }
        }

        public object DataContext
        {
            get
            {
                return this.GetValue(DataContextProperty);
            }

            set
            {
                this.SetValue(DataContextProperty, value);
            }
        }

        public string Header
        {
            get
            {
                return (string)this.GetValue(HeaderProperty);
            }

            set
            {
                this.SetValue(HeaderProperty, value);
            }
        }

        public object Icon
        {
            get
            {
                return this.GetValue(IconProperty);
            }

            set
            {
                this.SetValue(IconProperty, value);
            }
        }

        public string InputGestureText
        {
            get
            {
                return (string)this.GetValue(InputGestureTextProperty);
            }

            set
            {
                this.SetValue(InputGestureTextProperty, value);
            }
        }

        public bool IsCheckable
        {
            get
            {
                return (bool)this.GetValue(IsCheckableProperty);
            }

            set
            {
                this.SetValue(IsCheckableProperty, value);
            }
        }

        public bool IsChecked
        {
            get
            {
                return (bool)this.GetValue(IsCheckedProperty);
            }

            set
            {
                this.SetValue(IsCheckedProperty, value);
            }
        }

        public Binding IsCheckedBinding
        {
            get
            {
                return (Binding)this.GetValue(IsCheckedBindingProperty);
            }

            set
            {
                this.SetValue(IsCheckedBindingProperty, value);
            }
        }

        public MenuEntryObservableCollection Items
        {
            get
            {
                return (MenuEntryObservableCollection)this.GetValue(ItemsProperty);
            }

            set
            {
                this.SetValue(ItemsProperty, value);
            }
        }

        public double Priority { get; set; }

        #endregion

        #region Operators

        public static implicit operator MenuEntry(MenuItem victim)
        {
            var converted = new MenuEntry(victim.Header.ToString(), 0, victim.Command);

            foreach (object child in victim.Items)
            {
                if (child is MenuItem)
                {
                    converted.Items.Add(child as MenuItem);
                }
            }

            return converted;
        }

        public static implicit operator MenuEntry(Separator victim)
        {
            var converted = new MenuEntry("[Separator]");
            return converted;
        }

        public static implicit operator MenuEntry(MenuSeparator victim)
        {
            var converted = new MenuEntry("[Separator]");
            converted.Priority = victim.Priority;
            return converted;
        }

        #endregion

        #region Methods

        public override string ToString()
        {
            return this.Header + "(" + this.Priority + ")";
        }

        internal void MergeWith(MenuEntry menu)
        {
            foreach (MenuEntry item in menu.Items)
            {
                bool isAlreadyInItems = false;

                foreach (MenuEntry ownItem in this.Items)
                {

                    if (ownItem.Header == "[Separator]")
                    {
                        // Don't merge separators
                        continue;
                    }

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

        protected void OnPropertyChanged(string info)
        {
            if (this.PropertyChanged != null)
            {
                this.PropertyChanged(this, new PropertyChangedEventArgs(info));
            }
        }

        #endregion
    }
}
