using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace GeoGen_Studio
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
            

            this.Close();
        }

        private void Settings_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (Main.Get().GetConfig().enable3d)
            {
                Main main = Main.Get();
                main.SetupViewport();
                main.RebuildTerrain(null);
                main.viewport.Invalidate();
            }
        }
    }
}
