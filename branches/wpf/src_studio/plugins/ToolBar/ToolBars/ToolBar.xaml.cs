namespace GeoGen.Studio.PlugIns.ToolBars
{
	using System.Windows;
	using System.Windows.Controls;
	using System.Windows.Controls.Primitives;
	using GeoGen.Studio.PlugIns.Interfaces;

	/// <summary>
	/// A horizontal tool bar which displays items provided by other plug-ins.
	/// </summary>
	public partial class ToolBar : UserControl, IToolBar
	{
		private static readonly DependencyProperty itemsProperty = DependencyProperty.Register(
			"Items", typeof(ToolBarEntryObservableCollection), typeof(ToolBar));

		public ToolBarEntryObservableCollection Items
		{
			get
			{
				return (ToolBarEntryObservableCollection)GetValue(itemsProperty);
			}
			set
			{
				SetValue(itemsProperty, value);
			}
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="ToolBar"/> class.
		/// </summary>
		public ToolBar()
		{
			this.Items = new ToolBarEntryObservableCollection();

			InitializeComponent();
		}

		/// <summary>
		/// Adds one <see cref="ToolBarEntry"/> to the tool bar.
		/// </summary>
		/// <param name="ToolBar">The item.</param>
		public void AddItem(ToolBarEntry item)
		{
			// Null IS allowed (for simplicity's sake)
			if (item == null) return;

			this.Items.Add(item);
		}

		/// <summary>
		/// Adds one <see cref="Button"/> to the tool bar.
		/// </summary>
		/// <param name="entry">The item.</param>
		public void AddItem(Button entry)
		{
			this.AddItem((ToolBarEntry)entry);
		}

		/// <summary>
		/// Adds one <see cref="Separator"/> to the tool bar.
		/// </summary>
		/// <param name="entry">The item.</param>
		public void AddItem(Separator entry)
		{
			this.AddItem((ToolBarEntry)entry);
		}

		/// <summary>
		/// Adds one <see cref="ToggleButton"/> to the tool bar.
		/// </summary>
		/// <param name="entry">The item.</param>
		public void AddItem(ToggleButton entry)
		{
			this.AddItem((ToolBarEntry)entry);
		}
	}
}
