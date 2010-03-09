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
    public partial class LightSettings : Form
    {
        Config config = null;

        bool azimuthMouse = false;
        bool elevationMouse = false;

        public LightSettings()
        {
            InitializeComponent();
        }

        private void LightSettings_Load(object sender, EventArgs e)
        {
        //    this.viewportManager = Main.Get().GetViewportManager();
            this.config = Main.Get().GetConfig();

            this.lightEnabled.Checked = this.config.lightEnabled;
            this.azimuth.Text = (Math.Round(this.config.lightAzimuth * 180f / Math.PI, 1)).ToString();
            this.elevation.Text = (Math.Round(this.config.lightElevation * 180f / Math.PI, 1)).ToString();
            
            this.color1.BackColor = System.Drawing.Color.FromArgb((int)(255f * this.config.ambientLightColor.X), (int)(255f * this.config.ambientLightColor.Y), (int)(255f * this.config.ambientLightColor.Z));
            this.colorDialog1.Color = this.color1.BackColor;

            this.color2.BackColor = System.Drawing.Color.FromArgb((int)(255f * this.config.directionalLightColor.X), (int)(255f * this.config.directionalLightColor.Y), (int)(255f * this.config.directionalLightColor.Z));
            this.colorDialog2.Color = this.color2.BackColor;
        }

        private void azimuthGraphic_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;

            //e.Graphics.FillRectangle(Brushes.White, 0, 0, 173, 173);
            e.Graphics.DrawLine(new Pen(Color.DarkGray), new Point(86, 10), new Point(86, 163));
            e.Graphics.DrawLine(new Pen(Color.DarkGray), new Point(10, 86), new Point(163, 86));

            int sunX = (int) (Math.Cos(-this.config.lightAzimuth) * 68);
            int sunY = (int) (Math.Sin(-this.config.lightAzimuth) * 68);

            //Pen glare = new Pen(Pens.Yellow);
            //glare.PenType = System.Drawing.Drawing2D.PenType.

            e.Graphics.DrawLine(Pens.Khaki, new Point(86, 86), new Point(86 + sunX, 86 + sunY));

            e.Graphics.FillEllipse(Brushes.Yellow, 86 + sunX - 5, 86 + sunY - 5, 10, 10);
            e.Graphics.DrawEllipse(Pens.Gold, 86 + sunX - 5, 86 + sunY - 5, 10, 10);
            
        }

        private void azimuth_ValueChanged(object sender, EventArgs e)
        {
            if (this.azimuth.Value >= 360)
            {
                this.azimuth.Value -= 360;
            }
            else if (this.azimuth.Value < 0){
                this.azimuth.Value += 360;
            }

            this.config.lightAzimuth = (float) ((float)this.azimuth.Value * Math.PI / 180f);

            this.azimuthGraphic.Refresh();
            this.elevationGraphic.Refresh();
        }

        private void azimuthGraphic_MouseDown(object sender, MouseEventArgs e)
        {
            this.azimuthMouse = true;
        }

        private void azimuthGraphic_MouseUp(object sender, MouseEventArgs e)
        {
            this.azimuthMouse = false;
        }

        private void azimuthGraphic_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.azimuthMouse)
            {
                this.azimuth.Value = (decimal) (Math.Atan(-((double)(e.Y - 86) / (double)(e.X - 86))) * 180f / Math.PI) + (e.X < 86 ? 180 : 0);

            }
        }

        private void elevationGraphic_Paint(object sender, PaintEventArgs e)
        {
            int sunX = (int)(Math.Cos(-this.config.lightElevation) * 140);
            int sunY = (int)(Math.Sin(-this.config.lightElevation) * 140);

            if (this.azimuth.Value <= 90 || this.azimuth.Value >= 270)
            {
                e.Graphics.DrawLine(Pens.Khaki, new Point(36, 153), new Point(36 + sunX, 153 + sunY));

                e.Graphics.FillEllipse(Brushes.Yellow, 36 + sunX - 5, 153 + sunY - 5, 10, 10);
                e.Graphics.DrawEllipse(Pens.Gold, 36 + sunX - 5, 153 + sunY - 5, 10, 10);
            }
            else
            {
                e.Graphics.DrawLine(Pens.Khaki, new Point(189, 153), new Point(189 - sunX, 153 + sunY));

                e.Graphics.FillEllipse(Brushes.Yellow, 189 - sunX - 5, 153 + sunY - 5, 10, 10);
                e.Graphics.DrawEllipse(Pens.Gold, 189 - sunX - 5, 153 + sunY - 5, 10, 10);
            }

            e.Graphics.FillRectangle(new System.Drawing.Drawing2D.HatchBrush(System.Drawing.Drawing2D.HatchStyle.BackwardDiagonal, Color.DarkGreen, Color.White), 0, 153, 225, 20);
            e.Graphics.DrawLine(Pens.DarkGreen, new Point(0, 153), new Point(225, 153));
        }

        private void elevation_ValueChanged(object sender, EventArgs e)
        {
            this.config.lightElevation = (float)((float)this.elevation.Value * Math.PI / 180f);

            this.elevationGraphic.Refresh();
        }

        private void elevationGraphic_MouseDown(object sender, MouseEventArgs e)
        {
            this.elevationMouse = true;
        }

        private void elevationGraphic_MouseUp(object sender, MouseEventArgs e)
        {
            this.elevationMouse = false;
        }

        private void elevationGraphic_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.elevationMouse)
            {
                if (this.azimuth.Value <= 90 || this.azimuth.Value >= 270)
                {
                    this.elevation.Value = Math.Max(4, Math.Min(90, (decimal)(Math.Atan(-((double)(e.Y - 153) / (double)(e.X - 36))) * 180f / Math.PI) + (e.X < 36 ? 180 : 0)));
                }
                else
                {
                    this.elevation.Value = Math.Max(4, Math.Min(90, (decimal)(Math.Atan(((double)(e.Y - 153) / (double)(e.X - 189))) * 180f / Math.PI) + (e.X > 189 ? 180 : 0)));
                }
            }
        }

        private void color1_Click(object sender, EventArgs e)
        {
            this.colorDialog1.Color = this.color1.BackColor;

            if (this.colorDialog1.ShowDialog() == DialogResult.OK)
            {
                this.color1.BackColor = this.colorDialog1.Color;

                this.config.ambientLightColor.X = (float)this.color1.BackColor.R / 255f;
                this.config.ambientLightColor.Y = (float)this.color1.BackColor.G / 255f;
                this.config.ambientLightColor.Z = (float)this.color1.BackColor.B / 255f;
            }
        }

        private void color2_Click(object sender, EventArgs e)
        {
            this.colorDialog2.Color = this.color2.BackColor;

            if (this.colorDialog2.ShowDialog() == DialogResult.OK)
            {
                this.color2.BackColor = this.colorDialog2.Color;

                this.config.directionalLightColor.X = (float)this.color2.BackColor.R / 255f;
                this.config.directionalLightColor.Y = (float)this.color2.BackColor.G / 255f;
                this.config.directionalLightColor.Z = (float)this.color2.BackColor.B / 255f;
            }
        }
    }
}
