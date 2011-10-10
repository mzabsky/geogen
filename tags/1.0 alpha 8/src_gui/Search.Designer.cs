namespace GeoGen.Studio
{
    partial class Search
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.replace = new System.Windows.Forms.Button();
            this.findNext = new System.Windows.Forms.Button();
            this.replaceAll = new System.Windows.Forms.Button();
            this.findPrevious = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.matchCase = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(30, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Find:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 56);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(50, 13);
            this.label2.TabIndex = 0;
            this.label2.Text = "Replace:";
            // 
            // replace
            // 
            this.replace.Enabled = false;
            this.replace.Location = new System.Drawing.Point(217, 120);
            this.replace.Name = "replace";
            this.replace.Size = new System.Drawing.Size(75, 23);
            this.replace.TabIndex = 6;
            this.replace.Text = "Replace";
            this.replace.UseVisualStyleBackColor = true;
            this.replace.Click += new System.EventHandler(this.replace_Click);
            this.replace.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Search_KeyDown);
            // 
            // findNext
            // 
            this.findNext.Enabled = false;
            this.findNext.Location = new System.Drawing.Point(136, 120);
            this.findNext.Name = "findNext";
            this.findNext.Size = new System.Drawing.Size(75, 23);
            this.findNext.TabIndex = 5;
            this.findNext.Text = "Find Next";
            this.findNext.UseVisualStyleBackColor = true;
            this.findNext.Click += new System.EventHandler(this.findNext_Click);
            this.findNext.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Search_KeyDown);
            // 
            // replaceAll
            // 
            this.replaceAll.Enabled = false;
            this.replaceAll.Location = new System.Drawing.Point(298, 120);
            this.replaceAll.Name = "replaceAll";
            this.replaceAll.Size = new System.Drawing.Size(75, 23);
            this.replaceAll.TabIndex = 7;
            this.replaceAll.Text = "Replace All";
            this.replaceAll.UseVisualStyleBackColor = true;
            this.replaceAll.Click += new System.EventHandler(this.replaceAll_Click);
            this.replaceAll.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Search_KeyDown);
            // 
            // findPrevious
            // 
            this.findPrevious.Enabled = false;
            this.findPrevious.Location = new System.Drawing.Point(37, 120);
            this.findPrevious.Name = "findPrevious";
            this.findPrevious.Size = new System.Drawing.Size(93, 23);
            this.findPrevious.TabIndex = 4;
            this.findPrevious.Text = "Find Previous";
            this.findPrevious.UseVisualStyleBackColor = true;
            this.findPrevious.Click += new System.EventHandler(this.findPrevious_Click);
            this.findPrevious.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Search_KeyDown);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(68, 17);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(305, 20);
            this.textBox1.TabIndex = 1;
            this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            this.textBox1.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Search_KeyDown);
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(68, 53);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(305, 20);
            this.textBox2.TabIndex = 2;
            this.textBox2.TextChanged += new System.EventHandler(this.textBox2_TextChanged);
            this.textBox2.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Search_KeyDown);
            // 
            // matchCase
            // 
            this.matchCase.AutoSize = true;
            this.matchCase.Location = new System.Drawing.Point(68, 88);
            this.matchCase.Name = "matchCase";
            this.matchCase.Size = new System.Drawing.Size(83, 17);
            this.matchCase.TabIndex = 3;
            this.matchCase.Text = "Match Case";
            this.matchCase.UseVisualStyleBackColor = true;
            this.matchCase.CheckedChanged += new System.EventHandler(this.matchCase_CheckedChanged);
            this.matchCase.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Search_KeyDown);
            // 
            // Search
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(385, 155);
            this.Controls.Add(this.matchCase);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.findPrevious);
            this.Controls.Add(this.replaceAll);
            this.Controls.Add(this.findNext);
            this.Controls.Add(this.replace);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Search";
            this.ShowInTaskbar = false;
            this.Text = "Find and Replace";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Search_FormClosing);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Search_KeyDown);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button replace;
        private System.Windows.Forms.Button findNext;
        private System.Windows.Forms.Button replaceAll;
        private System.Windows.Forms.Button findPrevious;
        public System.Windows.Forms.TextBox textBox1;
        public System.Windows.Forms.TextBox textBox2;
        public System.Windows.Forms.CheckBox matchCase;
    }
}