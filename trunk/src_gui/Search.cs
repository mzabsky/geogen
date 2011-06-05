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
    public partial class Search : Form
    {
        public Search()
        {
            InitializeComponent();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            Main main = Main.Get();

            main.config.searchString = this.textBox1.Text;

            if (this.textBox1.Text.Length > 0)
            {
                main.findNextToolStripMenuItem.Enabled = true;
                main.findPrevToolStripMenuItem.Enabled = true;
                main.replaceToolStripMenuItem.Enabled = true;
                this.findNext.Enabled = true;
                this.findPrevious.Enabled = true;
                this.replace.Enabled = true;
                this.replaceAll.Enabled = true;
            }
            else
            {
                main.findNextToolStripMenuItem.Enabled = false;
                main.findPrevToolStripMenuItem.Enabled = false;
                main.replaceToolStripMenuItem.Enabled = false;
                this.findNext.Enabled = false;
                this.findPrevious.Enabled = false;
                this.replace.Enabled = false;
                this.replaceAll.Enabled = false;
            }
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            Main main = Main.Get();

            main.config.replaceString = this.textBox2.Text;
        }

        private void findNext_Click(object sender, EventArgs e)
        {
            Main main = Main.Get();

            main.findNextToolStripMenuItem_Click_1(sender, e);
        }

        private void findPrevious_Click(object sender, EventArgs e)
        {
            Main main = Main.Get();

            main.findPrevToolStripMenuItem_Click(sender, e);
        }

        private void Search_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.Hide();

            e.Cancel = true;
        }

        private void matchCase_CheckedChanged(object sender, EventArgs e)
        {
            Main main = Main.Get();

            main.config.searchMode = this.matchCase.Checked ? StringComparison.OrdinalIgnoreCase : StringComparison.Ordinal;
        }

        private void replace_Click(object sender, EventArgs e)
        {
            Main main = Main.Get();

            main.replaceToolStripMenuItem_Click(sender, e);
        }

        private void replaceAll_Click(object sender, EventArgs e)
        {
            Main.Get().ReplaceAll();
        }

        private void Search_KeyDown(object sender, KeyEventArgs e)
        {
            if(e.KeyCode == Keys.Escape){
                this.Close();
            }
        }
    }
}
