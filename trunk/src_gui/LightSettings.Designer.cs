namespace GeoGen_Studio
{
    partial class LightSettings
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LightSettings));
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.intensity2 = new System.Windows.Forms.TrackBar();
            this.label4 = new System.Windows.Forms.Label();
            this.color2 = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.listView1 = new System.Windows.Forms.ListView();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.intensity1 = new System.Windows.Forms.TrackBar();
            this.label3 = new System.Windows.Forms.Label();
            this.color1 = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.label7 = new System.Windows.Forms.Label();
            this.elevationGraphic = new System.Windows.Forms.PictureBox();
            this.azimuthGraphic = new System.Windows.Forms.PictureBox();
            this.label6 = new System.Windows.Forms.Label();
            this.lightEnabled = new System.Windows.Forms.CheckBox();
            this.colorDialog1 = new System.Windows.Forms.ColorDialog();
            this.colorDialog2 = new System.Windows.Forms.ColorDialog();
            this.azimuth = new System.Windows.Forms.NumericUpDown();
            this.elevation = new System.Windows.Forms.NumericUpDown();
            this.groupBox1.SuspendLayout();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.intensity2)).BeginInit();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.intensity1)).BeginInit();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.elevationGraphic)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.azimuthGraphic)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.azimuth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.elevation)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.groupBox3);
            this.groupBox1.Controls.Add(this.listView1);
            this.groupBox1.Controls.Add(this.groupBox2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(13, 36);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(420, 201);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Color and intensity";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.intensity2);
            this.groupBox3.Controls.Add(this.label4);
            this.groupBox3.Controls.Add(this.color2);
            this.groupBox3.Controls.Add(this.label5);
            this.groupBox3.Location = new System.Drawing.Point(214, 108);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(200, 83);
            this.groupBox3.TabIndex = 4;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Directional light";
            // 
            // intensity2
            // 
            this.intensity2.AutoSize = false;
            this.intensity2.Location = new System.Drawing.Point(68, 48);
            this.intensity2.Name = "intensity2";
            this.intensity2.Size = new System.Drawing.Size(132, 25);
            this.intensity2.TabIndex = 3;
            this.intensity2.TickStyle = System.Windows.Forms.TickStyle.None;
            this.intensity2.Value = 5;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(7, 52);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(49, 13);
            this.label4.TabIndex = 2;
            this.label4.Text = "Intensity:";
            // 
            // color2
            // 
            this.color2.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.color2.Location = new System.Drawing.Point(74, 17);
            this.color2.Name = "color2";
            this.color2.Size = new System.Drawing.Size(120, 18);
            this.color2.TabIndex = 1;
            this.color2.UseVisualStyleBackColor = true;
            this.color2.Click += new System.EventHandler(this.color2_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(7, 20);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(34, 13);
            this.label5.TabIndex = 0;
            this.label5.Text = "Color:";
            // 
            // listView1
            // 
            this.listView1.Location = new System.Drawing.Point(9, 35);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(199, 156);
            this.listView1.TabIndex = 0;
            this.listView1.UseCompatibleStateImageBehavior = false;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.intensity1);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.color1);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Location = new System.Drawing.Point(214, 19);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(200, 83);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Ambient light";
            // 
            // intensity1
            // 
            this.intensity1.AutoSize = false;
            this.intensity1.Location = new System.Drawing.Point(68, 48);
            this.intensity1.Name = "intensity1";
            this.intensity1.Size = new System.Drawing.Size(132, 25);
            this.intensity1.TabIndex = 3;
            this.intensity1.TickStyle = System.Windows.Forms.TickStyle.None;
            this.intensity1.Value = 5;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(7, 52);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(49, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "Intensity:";
            // 
            // color1
            // 
            this.color1.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.color1.Location = new System.Drawing.Point(74, 17);
            this.color1.Name = "color1";
            this.color1.Size = new System.Drawing.Size(120, 18);
            this.color1.TabIndex = 1;
            this.color1.UseVisualStyleBackColor = true;
            this.color1.Click += new System.EventHandler(this.color1_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(7, 20);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(34, 13);
            this.label2.TabIndex = 0;
            this.label2.Text = "Color:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 19);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(45, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Presets:";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.elevation);
            this.groupBox4.Controls.Add(this.azimuth);
            this.groupBox4.Controls.Add(this.label7);
            this.groupBox4.Controls.Add(this.elevationGraphic);
            this.groupBox4.Controls.Add(this.azimuthGraphic);
            this.groupBox4.Controls.Add(this.label6);
            this.groupBox4.Location = new System.Drawing.Point(13, 243);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(420, 225);
            this.groupBox4.TabIndex = 1;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Direction";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(186, 20);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(54, 13);
            this.label7.TabIndex = 3;
            this.label7.Text = "Elevation:";
            // 
            // elevationGraphic
            // 
            this.elevationGraphic.BackColor = System.Drawing.Color.White;
            this.elevationGraphic.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.elevationGraphic.Location = new System.Drawing.Point(189, 43);
            this.elevationGraphic.Name = "elevationGraphic";
            this.elevationGraphic.Size = new System.Drawing.Size(225, 173);
            this.elevationGraphic.TabIndex = 2;
            this.elevationGraphic.TabStop = false;
            this.elevationGraphic.MouseMove += new System.Windows.Forms.MouseEventHandler(this.elevationGraphic_MouseMove);
            this.elevationGraphic.MouseDown += new System.Windows.Forms.MouseEventHandler(this.elevationGraphic_MouseDown);
            this.elevationGraphic.Paint += new System.Windows.Forms.PaintEventHandler(this.elevationGraphic_Paint);
            this.elevationGraphic.MouseUp += new System.Windows.Forms.MouseEventHandler(this.elevationGraphic_MouseUp);
            // 
            // azimuthGraphic
            // 
            this.azimuthGraphic.BackColor = System.Drawing.Color.White;
            this.azimuthGraphic.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.azimuthGraphic.Location = new System.Drawing.Point(6, 43);
            this.azimuthGraphic.Name = "azimuthGraphic";
            this.azimuthGraphic.Size = new System.Drawing.Size(173, 173);
            this.azimuthGraphic.TabIndex = 1;
            this.azimuthGraphic.TabStop = false;
            this.azimuthGraphic.MouseMove += new System.Windows.Forms.MouseEventHandler(this.azimuthGraphic_MouseMove);
            this.azimuthGraphic.MouseDown += new System.Windows.Forms.MouseEventHandler(this.azimuthGraphic_MouseDown);
            this.azimuthGraphic.Paint += new System.Windows.Forms.PaintEventHandler(this.azimuthGraphic_Paint);
            this.azimuthGraphic.MouseUp += new System.Windows.Forms.MouseEventHandler(this.azimuthGraphic_MouseUp);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 20);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(47, 13);
            this.label6.TabIndex = 0;
            this.label6.Text = "Azimuth:";
            // 
            // lightEnabled
            // 
            this.lightEnabled.AutoSize = true;
            this.lightEnabled.Location = new System.Drawing.Point(13, 13);
            this.lightEnabled.Name = "lightEnabled";
            this.lightEnabled.Size = new System.Drawing.Size(95, 17);
            this.lightEnabled.TabIndex = 2;
            this.lightEnabled.Text = "Enable lighting";
            this.lightEnabled.UseVisualStyleBackColor = true;
            // 
            // colorDialog1
            // 
            this.colorDialog1.FullOpen = true;
            // 
            // colorDialog2
            // 
            this.colorDialog2.FullOpen = true;
            // 
            // azimuth
            // 
            this.azimuth.DecimalPlaces = 1;
            this.azimuth.Location = new System.Drawing.Point(59, 18);
            this.azimuth.Maximum = new decimal(new int[] {
            360,
            0,
            0,
            0});
            this.azimuth.Minimum = new decimal(new int[] {
            360,
            0,
            0,
            -2147483648});
            this.azimuth.Name = "azimuth";
            this.azimuth.Size = new System.Drawing.Size(60, 20);
            this.azimuth.TabIndex = 6;
            this.azimuth.ValueChanged += new System.EventHandler(this.azimuth_ValueChanged);
            // 
            // elevation
            // 
            this.elevation.DecimalPlaces = 1;
            this.elevation.Location = new System.Drawing.Point(246, 18);
            this.elevation.Maximum = new decimal(new int[] {
            90,
            0,
            0,
            0});
            this.elevation.Minimum = new decimal(new int[] {
            4,
            0,
            0,
            0});
            this.elevation.Name = "elevation";
            this.elevation.Size = new System.Drawing.Size(60, 20);
            this.elevation.TabIndex = 7;
            this.elevation.Value = new decimal(new int[] {
            4,
            0,
            0,
            0});
            this.elevation.ValueChanged += new System.EventHandler(this.elevation_ValueChanged);
            // 
            // LightSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ControlLight;
            this.ClientSize = new System.Drawing.Size(445, 480);
            this.Controls.Add(this.lightEnabled);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox1);
            this.DoubleBuffered = true;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "LightSettings";
            this.ShowInTaskbar = false;
            this.Text = "Light settings";
            this.Load += new System.EventHandler(this.LightSettings_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.intensity2)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.intensity1)).EndInit();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.elevationGraphic)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.azimuthGraphic)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.azimuth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.elevation)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TrackBar intensity1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button color1;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.TrackBar intensity2;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button color2;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.PictureBox azimuthGraphic;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.PictureBox elevationGraphic;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.CheckBox lightEnabled;
        private System.Windows.Forms.ColorDialog colorDialog1;
        private System.Windows.Forms.ColorDialog colorDialog2;
        private System.Windows.Forms.NumericUpDown azimuth;
        private System.Windows.Forms.NumericUpDown elevation;
    }
}