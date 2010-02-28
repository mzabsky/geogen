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
    public partial class Export : Form
    {
        public Export()
        {
            InitializeComponent();
        }

        private void cancel_Click(object sender, EventArgs e)
        {

        }

        private void reset_Click(object sender, EventArgs e)
        {
            
        }

        private void exportButton_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }
    }
}
