namespace GeoGen.Studio.PlugIns.MenuBars
{
	using System;
	using System.Collections.Generic;
	using System.Windows;
	using System.Windows.Controls;
	using System.Windows.Data;
	using System.Windows.Input;
	using System.Windows.Markup;
	using System.Windows.Media;
	using System.Windows.Media.Imaging;	
	using GeoGen.Studio.Utilities.Collections;

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
			"Icon", typeof(object), typeof(MenuEntry), new PropertyMetadata());

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

		private static readonly DependencyProperty ItemsProperty = DependencyProperty.Register(
			"Items", typeof(MenuEntryObservableCollection), typeof(MenuEntry), new PropertyMetadata());

		public MenuEntryObservableCollection Items
		{
			get
			{
				return (MenuEntryObservableCollection)GetValue(ItemsProperty);
			}
			set
			{
				SetValue(ItemsProperty, value);
			}
		}

		private static readonly DependencyProperty IsCheckedBindingProperty = DependencyProperty.Register(
			"IsCheckedBinding", typeof(Binding), typeof(MenuEntry), new PropertyMetadata(null));

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

		private static readonly DependencyProperty IsCheckableProperty = DependencyProperty.Register(
			"IsCheckable", typeof(bool), typeof(MenuEntry), new PropertyMetadata(null));

		public bool IsCheckable
		{
			get
			{
				return (bool)GetValue(IsCheckableProperty);
			}
			set
			{
				SetValue(IsCheckableProperty, value);
			}
		}

		private static readonly DependencyProperty IsCheckedProperty = DependencyProperty.Register(
			"IsChecked", typeof(bool), typeof(MenuEntry), new PropertyMetadata(null));

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

		private static readonly DependencyProperty DataContextProperty = DependencyProperty.Register(
			"DataContext", typeof(object), typeof(MenuEntry), new PropertyMetadata(null));

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

		public MenuEntry() {
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
			MenuEntryObservableCollection items = null
		)
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
			
			if(icon != null){
				if(icon is string){
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

			}

			if(IsCheckedBinding != null)
			{
				if(isCheckedBinding.Source == null){
					isCheckedBinding.Source = dataContext;
				}

				BindingOperations.SetBinding(this, MenuEntry.IsCheckedProperty, this.IsCheckedBinding);
			}
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

		public static implicit operator MenuEntry(Separator victim)
		{
			MenuEntry converted = new MenuEntry("[Separator]");
			return converted;
		}

		public static implicit operator MenuEntry(MenuSeparator victim)
		{
			MenuEntry converted = new MenuEntry("[Separator]");
			converted.Priority = victim.Priority;
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
