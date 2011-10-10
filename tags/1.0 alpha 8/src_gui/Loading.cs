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
    public partial class Loading : Form
    {
        public Loading()
        {
            InitializeComponent();
        }

        private void Loading_Load(object sender, EventArgs e)
        {
            /*new System.Threading.Thread((System.Threading.ThreadStart) delegate
            {
                Main main = new Main();
                main.loader = this;
                main.Show();
            }).Start();*/
        }

        public void FadeOut(){
            opacityTimer.Enabled = true;
        }

        private void opacityTimer_Tick(object sender, EventArgs e)
        {
            Message.Visible = false;

            if (this.Opacity == 0)
            {
                //this.Close();
                opacityTimer.Enabled = false;
            }
            else
            {
                this.Opacity -= 0.05;
            }
        }
    }
}
