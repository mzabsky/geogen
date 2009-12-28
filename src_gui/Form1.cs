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
    public partial class Main : Form
    {
        public enum Tabs
        {
            Code = 0,
            Output2D = 1,
            Log = 2,
            Output3D = 4,
            Docs = 5
        };

        private bool needsSaving;
        public ProcessManager processManager;
        public OutputManager outputManager;
        private Config config;
        private string currentFileName;
        private bool scrollOutput;
        int outputLastMouseX;
        int outputLastMouseY;

        // MSVS generated stuff
        public Main()
        {
            InitializeComponent();
        }

        // form initializer
        private void Main_Load(object sender, EventArgs e)
        {
            this.needsSaving = false;

            this.config = Config.Load();

            this.processManager = new ProcessManager();
            this.outputManager = new OutputManager();

            this.currentFileName = "";

            this.editor.TextInserted += editor_TextInserted;
            this.editor.TextDeleted += editor_TextInserted;

            // trigger the "new file" button click, so the template is loaded (without doubling the code)
            this.newToolStripMenuItem_Click(sender, e);

            // make the output picturebox have zero size (no output is available)
            this.output.Width = 0;
            this.output.Height = 0;

            this.consoleLarge.Text = this.console.Text;

            this.outputManager.LoadOverlays();

            this.scrollOutput = false;

            this.parameters.SelectedObject = parameters.Item;

            this.editor.ConfigurationManager.CustomLocation = this.config.ScintillaDefinitonsFile;
        }

        // this app's GetInstance()
        public static Main Get()
        {
            return (Main) System.Windows.Forms.Application.OpenForms[0];
        }

        public Config GetConfig()
        {
            return this.config;
        }

        public ProcessManager GetProcessManager()
        {
            return this.processManager;
        }

        public OutputManager GetOutputManager()
        {
            return this.outputManager;
        }

        public string GetScript()
        {
            return this.editor.Text;
        }

        // try to terminate the application
        private bool Quit()
        {
            // save the file if requested
            if (!this.TryToSaveIfUnsaved()) return false;

            this.config.Save();

            return true;
        }

        private bool TryToSaveIfUnsaved()
        {
            if (this.needsSaving)
            {
                // ask the user
                System.Windows.Forms.DialogResult result = System.Windows.Forms.MessageBox.Show("Do you wish to save unsaved content?", "", System.Windows.Forms.MessageBoxButtons.YesNoCancel);

                switch (result)
                {
                    case System.Windows.Forms.DialogResult.No: break;
                    case System.Windows.Forms.DialogResult.Cancel: return false;
                    case System.Windows.Forms.DialogResult.Yes:
                        {
                            if (this.saveFile.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                            {
                                System.IO.File.WriteAllText(this.saveFile.FileName, this.editor.Text);
                                this.openFile.FileName = this.saveFile.FileName;
                                this.currentFileName = this.saveFile.FileName;

                                break;
                            }
                            else
                            {
                                return false;
                            }
                        }
                }
            }

            return true;
        }

        public void WriteToConsole(string str)
        {
            this.console.Text += str + Environment.NewLine;
        }

        public void ButtonsRunMode()
        {
            this.executeToolStripButton.Enabled = false;
            this.executeToolStripMenuItem.Enabled = false;
            this.terminateToolStripButton.Enabled = true;
            this.terminateToolStripMenuItem.Enabled = true;
            this.benchmarkToolStripButton.Enabled = false;
            this.benchmarkToolStripMenuItem.Enabled = false;
        }

        public void ButtonsNoRunMode()
        {
            this.executeToolStripButton.Enabled = true;
            this.executeToolStripMenuItem.Enabled = true;
            this.terminateToolStripButton.Enabled = false;
            this.terminateToolStripMenuItem.Enabled = false;
            this.benchmarkToolStripButton.Enabled = true;
            this.benchmarkToolStripMenuItem.Enabled = true;
        }

        public void OutputButtonsOn()
        {
            this.saveOutput.Enabled = true;
            this.clearOutputs.Enabled = true;
            this.outputs.Enabled = true;
            this.overlays.Enabled = true;
            this.toggleOverlay.Enabled = true;
            this.refreshOverlays.Enabled = true;
        }

        public void OutputButtonsOff()
        {
            this.saveOutput.Enabled = false;
            this.clearOutputs.Enabled = false;
            this.outputs.Enabled = false;
            this.overlays.Enabled = false;
            this.toggleOverlay.Enabled = false;
            this.refreshOverlays.Enabled = false;
        }

        public void SelectTab(Tabs tab)
        {
            this.tabs.SelectedIndex = (int)tab;
        }

        public void SetStatus(string str)
        {
            this.status.Text = str;
        }

        public void SetErrorStatus(bool isError)
        {
            if (isError)
            {
                this.console.BackColor = Color.LightCoral;

                this.verifyToolStripButton.Enabled = true;
                this.verifyToolStripMenuItem.Enabled = true;
            }
            else
            {
                this.console.BackColor = System.Drawing.SystemColors.ControlLight;

                this.verifyToolStripButton.Enabled = false;
                this.verifyToolStripMenuItem.Enabled = false;
            }
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // save the file if requested
            if (!this.TryToSaveIfUnsaved()) return;

            // clear the editor text (in case loading template failed)
            this.editor.Text = "";

            try
            {
                this.editor.Text = System.IO.File.ReadAllText(this.config.TemplateFile);
            }
            catch (System.Exception)
            {
                this.WriteToConsole("Could not load file template.");
            }

            // creating new file should not be undoable
            this.editor.UndoRedo.EmptyUndoBuffer();

            // make sure the file won't overwrite the previously edited file when hitting Ctrl + S
            this.currentFileName = "";

            this.needsSaving = false;
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // save the file if requested
            if (!this.TryToSaveIfUnsaved()) return;

            if (this.openFile.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                this.editor.Text = System.IO.File.ReadAllText(this.openFile.FileName);
                this.saveFile.FileName = this.openFile.FileName;
                this.currentFileName = this.openFile.FileName;
                this.needsSaving = false;

                this.editor.UndoRedo.EmptyUndoBuffer();
                this.editor.UndoRedo.BeginUndoAction();
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // trigger the Save As action iif the current file is not known
            if (this.currentFileName == "")
            {
                this.saveAsToolStripMenuItem_Click(sender, e);
            }
            else
            {
                System.IO.File.WriteAllText(this.currentFileName, this.editor.Text);

                this.needsSaving = false;
            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.saveFile.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                System.IO.File.WriteAllText(this.saveFile.FileName, this.editor.Text);

                this.openFile.FileName = this.saveFile.FileName;
                this.currentFileName = this.saveFile.FileName;

                this.needsSaving = false;
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // ask the user if he really wants to quit
            if (this.Quit())
            {
                Application.Exit();
            }
        }

        private void Main_FormClosing(object sender, FormClosingEventArgs e)
        {
            e.Cancel = !this.Quit();
        }

        private void editor_TextInserted(object sender, ScintillaNet.TextModifiedEventArgs e)
        {
            this.processManager.ScheduleCheck();

            this.needsSaving = true;
        }

        private void executeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.processManager.ExecuteScript(editor.Text, false, "");
        }

        private void refreshOverlays_Click(object sender, EventArgs e)
        {
            this.outputManager.LoadOverlays();
        }

        private void toggleOverlay_Click(object sender, EventArgs e)
        {
            this.outputManager.ToggleOverlay();
        }

        private void overlays_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(this.overlays.SelectedIndex > 0) this.toggleOverlay.Checked = true;
            else this.toggleOverlay.Checked = false;

            this.outputManager.ShowImage();
        }

        private void outputs_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.outputManager.ShowImage();
        }

        private void console_TextChanged(object sender, EventArgs e)
        {
            this.consoleLarge.Text = this.console.Text;

            this.console.SelectionStart = this.console.Text.Length - this.console.Lines[this.console.Lines.Length - 1].Length;
            this.console.SelectionLength = 0;
            this.console.ScrollToCaret();

            this.consoleLarge.SelectionStart = this.consoleLarge.Text.Length - this.consoleLarge.Lines[this.consoleLarge.Lines.Length - 1].Length;
            this.consoleLarge.SelectionLength = 0;
            this.consoleLarge.ScrollToCaret();
        }

        private void output_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                this.scrollOutput = true;
                outputLastMouseX = e.X;
                outputLastMouseY = e.Y;
            }
        }

        private void output_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                this.scrollOutput = false;
            }
        }

        private void output_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.scrollOutput && this.output.Image != null)
            {
                this.output.Left += (e.X - this.outputLastMouseX);
                this.output.Top += (e.Y - this.outputLastMouseY);

                this.output.Update();
                this.outputContainer.Update();
            }

            System.Drawing.Bitmap bitmap = (System.Drawing.Bitmap)output.Image;

            if(e.X > 0 && e.Y > 0 && e.X < output.Image.Width && e.Y < output.Image.Height) this.coords.Text = e.X + " x " + e.Y + " [" + bitmap.GetPixel(e.X, e.Y).R + "]";
        }

        private void terminateToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.processManager.Terminate();
        }

        private void verifyToolStripButton_Click(object sender, EventArgs e)
        {
            this.processManager.ShowLastErrorInfo();
        }

        private void clearOutputs_Click(object sender, EventArgs e)
        {
            this.outputManager.ClearData();

            this.SelectTab(Tabs.Code);
        }

        private void saveOutput_Click(object sender, EventArgs e)
        {
            this.outputManager.SaveOutput();
        }

        private void benchmarkToolStripButton_Click(object sender, EventArgs e)
        {
            this.processManager.StartBenchmark();
        }

        private void undoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.editor.UndoRedo.Undo();
        }

        private void redoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.editor.UndoRedo.Redo();
        }

        private void cutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.editor.Clipboard.Cut();
        }

        private void copyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.editor.Clipboard.Copy();
        }

        private void pasteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.editor.Clipboard.Paste();
        }

        private void searchAndReplaceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.editor.FindReplace.ShowFind();
        }

        private void findNextToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //this.editor.FindReplace.IncrementalSearcher.
        }

        private void codeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.SelectTab(Tabs.Code);
        }

        private void outputToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.SelectTab(Tabs.Output2D);
        }

        private void logToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.SelectTab(Tabs.Log);
        }

        private void sidebarToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.splitContainer.Panel2Collapsed = !this.sidebarToolStripMenuItem.Checked;
        }

        private void toolbarToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.toolStrip.Visible = this.toolbarToolStripMenuItem.Checked;
        }

        private void statusbarToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.statusStrip.Visible = this.statusbarToolStripMenuItem.Checked;
        }

        private void scriptParametersToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.sidebarSplitContainer.Panel1Collapsed = !this.scriptParametersToolStripMenuItem.Checked;

            if (!this.scriptParametersToolStripMenuItem.Checked) this.consoleToolStripMenuItem.Checked = true;
        }

        private void consoleToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.sidebarSplitContainer.Panel2Collapsed = !this.consoleToolStripMenuItem.Checked;

            if (!this.consoleToolStripMenuItem.Checked) this.scriptParametersToolStripMenuItem.Checked = true;
        }

        private void leftToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.leftToolStripMenuItem.Checked = true;
            this.leftToolStripMenuItem.Checked = true;
            this.rightToolStripMenuItem.Checked = false;
            this.bottomToolStripMenuItem.Checked = false;

            this.splitContainer.Orientation = Orientation.Vertical;
            this.splitContainer.RightToLeft = RightToLeft.Yes;
            this.sidebarSplitContainer.Orientation = Orientation.Horizontal;
        }

        private void rightToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.leftToolStripMenuItem.Checked = false;
            this.rightToolStripMenuItem.Checked = true;
            this.bottomToolStripMenuItem.Checked = false;

            this.splitContainer.Orientation = Orientation.Vertical;
            this.splitContainer.RightToLeft = RightToLeft.No;
            this.sidebarSplitContainer.Orientation = Orientation.Horizontal;
        }

        private void bottomToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.leftToolStripMenuItem.Checked = false;
            this.rightToolStripMenuItem.Checked = false;
            this.bottomToolStripMenuItem.Checked = true;

            this.splitContainer.Orientation = Orientation.Horizontal;
            this.splitContainer.RightToLeft = RightToLeft.No;
            this.sidebarSplitContainer.Orientation = Orientation.Vertical;
        }

        private void wordWrapToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.editor.LineWrap.Mode = this.wordWrapToolStripMenuItem.Checked ? ScintillaNet.WrapMode.Word : ScintillaNet.WrapMode.None;
        }

        private void increaseFontSizeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.editor.Zoom++;
        }

        private void decreaseFontSizeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.editor.Zoom--;
        }

        private void resetFontSizeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.editor.Zoom = 0;
        }

        private void whiteSpaceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.editor.WhiteSpace.Mode = this.whiteSpaceToolStripMenuItem.Checked ? ScintillaNet.WhiteSpaceMode.VisibleAlways : ScintillaNet.WhiteSpaceMode.Invisible;
        }

        private void lineBreaksToolStripMenuItem_Click(object sender, EventArgs e)
        {
            editor.EndOfLine.IsVisible = this.lineBreaksToolStripMenuItem.Checked;
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            About about = new About();
            about.ShowDialog();
        }

        private void settingsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Settings settings = new Settings();
            settings.ShowDialog();
        }

          
    }
}
