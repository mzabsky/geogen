namespace GeoGen.Studio.UI
{
    using System.Windows;
    using System.Windows.Documents;
    using System.Windows.Media;

    /// <summary>
    /// <see cref="Adorner"/> indicating busy or unusable part of the UI.
    /// </summary>
    public class BusyAdorner : Adorner
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="BusyAdorner"/> class.
        /// </summary>
        /// <param name="adornerElement">
        /// The adorner element.
        /// </param>
        public BusyAdorner(UIElement adornerElement)
                : base(adornerElement)
        {           
        }

        /// <summary>
        /// Renders semi-transparent black over the adorner area.
        /// </summary>
        /// <param name="drawingContext">The drawing instructions for a specific element. This context is provided to the layout system.</param>
        protected override void OnRender(System.Windows.Media.DrawingContext drawingContext)
        {
            var brush = new SolidColorBrush(Colors.Black);
            brush.Opacity = 0.5;

            drawingContext.DrawRectangle(brush, null, new Rect(new Point(0, 0), this.RenderSize));
            base.OnRender(drawingContext);
        }
    }
}
