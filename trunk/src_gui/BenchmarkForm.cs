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
