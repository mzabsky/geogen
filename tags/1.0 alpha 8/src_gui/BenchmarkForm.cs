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
    public partial class BenchmarkForm : Form
    {
        public BenchmarkForm()
        {
            InitializeComponent();
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void BenchmarkForm_Load(object sender, EventArgs e)
        {
            this.button.Focus();
        }

        private void button_Click(object sender, EventArgs e)
        {
            Main main = Main.Get();

            if (main.InBenchmarkInProgress())
            {
                main.CancelBenchmark();

                this.Close();
            }
            else if (this.button.Text == "Start Benchmark")
            {
                this.button.Text = "Cancel Benchmark";

                main.Benchmark();
            }
            else
            {
                this.Close();
            }
        }
    }
}
