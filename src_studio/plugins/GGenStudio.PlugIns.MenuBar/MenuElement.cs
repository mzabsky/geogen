using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;

namespace GeoGen.Studio.PlugIns
{
    class MenuElement: System.Windows.Controls.Menu 
    {
        protected override DependencyObject GetContainerForItemOverride()
        {
            return new System.Windows.Controls.ContentPresenter();
        }
    }
}
