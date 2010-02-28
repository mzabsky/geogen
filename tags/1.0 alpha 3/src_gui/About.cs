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
