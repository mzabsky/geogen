namespace GeoGen.Studio.PlugIns.ToolBars
{
	using System;
	using System.Windows;
	using System.Windows.Controls;
	using System.Windows.Data;
	using System.Windows.Input;
	using System.Windows.Media.Imaging;

	public class ToolBarCheckableButton : ToolBarButton
	{
		private static readonly DependencyProperty IsCheckedBindingProperty = DependencyProperty.Register(
			"IsCheckedBinding", typeof(Binding), typeof(ToolBarCheckableButton), new PropertyMetadata(null));

		private static readonly DependencyProperty IsCheckedProperty = DependencyProperty.Register(
			"IsChecked", typeof(bool), typeof(ToolBarCheckableButton), new PropertyMetadata(null));

		static ToolBarCheckableButton()
		{
			DefaultStyleKeyProperty.OverrideMetadata(
				typeof(ToolBarCheckableButton),
				new FrameworkPropertyMetadata(typeof(ToolBarCheckableButton)));            
		}

		public ToolBarCheckableButton()
		{
		}

		public ToolBarCheckableButton(object icon, double priority = 0, ICommand command = null, object toolTip = null, bool isChecked = false, Binding isCheckedBinding = null, object dataContext = null)
		{
			// Check if the "icon" is a path to an image; if it is, load te image
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

			this.IsChecked = isChecked;
			this.IsCheckedBinding = isCheckedBinding;
			this.Priority = priority;
			this.Command = command;
			this.ToolTip = toolTip;
			this.DataContext = dataContext;

			if (this.IsCheckedBinding != null)
			{
				if (this.IsCheckedBinding.Source == null)
				{
					this.IsCheckedBinding.Source = this.DataContext;
				}

				BindingOperations.SetBinding(this, ToolBarCheckableButton.IsCheckedProperty, this.IsCheckedBinding);
			}
		}

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
	}
}
