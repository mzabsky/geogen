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
    public partial class Settings : Form
    {
        public Settings()
        {
            InitializeComponent();
        }

        private void Settings_Load(object sender, EventArgs e)
        {
            settingsGrid.SelectedObject = Main.Get().GetConfig();
        }

        private void save_Click(object sender, EventArgs e)
        {
            // the setings are automatically saved whenever a value is changed (behavior intrisnic to the property grid)
            // so we just close the form
            this.Close();
        }

        private void Settings_FormClosing(object sender, FormClosingEventArgs e)
        {
            Config config = Main.Get().GetConfig();


            // check if global map resolution is >= than model/texture resolutions
            if ((int)config.mapDetailLevel < (int)config.ModelDetailLevel || (int)config.mapDetailLevel < (int)config.TextureDetailLevel)
            {
                if (System.Windows.Forms.MessageBox.Show("The selected map resolution is lower than selected model and/or texture resolution. This will result in model and/or texture resolution being limited by the overall map detail level.", "GeoGen Studio Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Warning) == DialogResult.Cancel)
                {
                    e.Cancel = true;
                }
            }

            // the 3D view might need to reinitialize
            if (Main.Get().GetConfig().enable3d)
            {                
                Main main = Main.Get();
                main.Output3dButtonsOff();
                main.SetupViewport();
                main.RebuildTerrain(main.currentImportedFile);
                main.viewport.Invalidate();
            }
        }
    }
}
