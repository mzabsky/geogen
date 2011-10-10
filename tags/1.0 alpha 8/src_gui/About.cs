/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    GeoGen is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GeoGen.  If not, see <http://www.gnu.org/licenses/>.

*/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace GeoGen.Studio
{
    public partial class About : Form
    {
        bool modeIn = true; // fade in mode (fade out when false)

        public About()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("http://code.google.com/p/geogen/");
        }

        private void button_Click(object sender, EventArgs e)
        {
            this.opacityTimer.Enabled = true;
        }

        private void About_Load(object sender, EventArgs e)
        {

        }

        private void opacityTimer_Tick(object sender, EventArgs e)
        {
            if (modeIn == true)
            {
                this.Opacity += 0.04;

                if (this.Opacity == 1.0)
                {
                    this.opacityTimer.Enabled = false;
                    this.modeIn = false;
                }
            }
            else
            {
                this.Opacity -= 0.04;

                if (this.Opacity == 0.0)
                {
                    this.opacityTimer.Enabled = false;
                    this.Close();
                }
            }
        }
    }
}
