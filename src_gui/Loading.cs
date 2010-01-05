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

        }

        public void FadeOut(){
            opacityTimer.Enabled = true;
        }

        private void opacityTimer_Tick(object sender, EventArgs e)
        {
            Message.Visible = false;

            if (this.Opacity == 0)
            {
                this.Close();
            }
            else
            {
                this.Opacity -= 0.05;
            }
        }
    }
}
