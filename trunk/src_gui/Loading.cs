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
