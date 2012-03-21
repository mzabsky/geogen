namespace GeoGen.Studio.PlugIns
{
    using System.Windows;
    using System.Windows.Controls;

    /// <summary>
    /// Template selector for the Settings dialog.
    /// </summary>
    internal class SettingsPropertyItemTemplateSelector : DataTemplateSelector
    {
        /// <summary>
        /// Gets or sets data template for the integer type property.
        /// </summary>
        /// <value>
        /// The template.
        /// </value>
        public DataTemplate Int { get; set; }

        /// <summary>
        /// Gets or sets data template for the float type property.
        /// </summary>
        /// <value>
        /// The template.
        /// </value>
        public DataTemplate Float { get; set; }

        /// <summary>
        /// Gets or sets data template for the boolean type property.
        /// </summary>
        /// <value>
        /// The template.
        /// </value>
        public DataTemplate Bool { get; set; }

        /// <summary>
        /// Gets or sets data template for the string type property.
        /// </summary>
        /// <value>
        /// The template.
        /// </value>
        public DataTemplate String { get; set; }

        /// <summary>
        /// Gets or sets data template used when the type cannot be decided.
        /// </summary>
        /// <value>
        /// The template.
        /// </value>
        public DataTemplate UnknownType { get; set; }

        /// <summary>
        /// Returns a DataTemplate based on the type of the property.
        /// </summary>
        /// <param name="item">The data object for which to select the template.</param>
        /// <param name="container">The data-bound object.</param>
        /// <returns>
        /// Returns a <see cref="T:System.Windows.DataTemplate"/> or null. The default value is null.
        /// </returns>
        public override DataTemplate SelectTemplate(object item, DependencyObject container)
        {
            var viewModel = (SettingsPropertyViewModel)item;

            if (
                viewModel.Value is int ||
                viewModel.Value is uint ||
                viewModel.Value is long ||
                viewModel.Value is ulong ||
                viewModel.Value is short ||
                viewModel.Value is ushort ||
                viewModel.Value is byte ||
                viewModel.Value is float ||
                viewModel.Value is double)
            {
                return this.Int;
            }
            else if (
                viewModel.Value is float || 
                viewModel.Value is double
            )
            {
                return this.Float;
            }
            else if (viewModel.Value is bool)
            {
                return this.Bool;
            }
            else if (viewModel.Value is string)
            {
                return this.String;
            }
            else
            {
                return this.UnknownType;
            }
        }
    }
}
