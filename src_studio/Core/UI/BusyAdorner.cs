using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media;
using System.Windows;
using System.Windows.Documents;

namespace GeoGen.Studio.UI
{
    public class BusyAdorner: Adorner
    {
        public BusyAdorner(UIElement adornerElement)
                : base(adornerElement)
        { }

        protected override void OnRender(System.Windows.Media.DrawingContext drawingContext)
        {
            SolidColorBrush brush = new SolidColorBrush(Colors.Black);
            brush.Opacity = 0.5;

            drawingContext.DrawRectangle(brush, null,
                        new Rect(new Point(0, 0), DesiredSize));
            base.OnRender(drawingContext);
        }
    }
}
