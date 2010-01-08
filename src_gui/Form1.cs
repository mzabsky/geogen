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
            Output3D = 2,
            Log = 3,
            Docs = 4
        };

        public enum SidebarMode
        {
            Left,
            Right,
            Bottom
        };

        public enum ActionAfterExectution
        {
            DoNothing = 0,
            GoTo2DOutput = 1,
            GoTo3DOutput = 2
        };

        private bool needsSaving;
        public ProcessManager processManager;
        public OutputManager outputManager;
        public ViewportManager viewportManager;
        public Config config;
        private string currentFileName;
        private bool scrollOutput;
        private bool scrollViewport; // viewport azimuth and elevatio mode (left mouse is clicked down)
        private bool moveViewport; // viewport target movement mode (right mouse is clicked down)
        int outputLastMouseX;
        int outputLastMouseY;
        int mouseDownX;
        int mouseDownY;
        bool outputMouse;
        bool viewportMouse;
        SidebarMode sidebarMode = SidebarMode.Right;
        List<string> statuses = new List<string>();

        public Loading loader = null;

        // MSVS generated stuff
        public Main()
        {
            InitializeComponent();
        }

        // form initializer
        private void Main_Load(object sender, EventArgs e)
        {
            // show the splash screen
            Loading loading = new Loading();
            loading.Show();
            loading.Refresh();

            this.needsSaving = false;

            // initialize the managers
            this.processManager = new ProcessManager();
            this.outputManager = new OutputManager();
            this.viewportManager = new ViewportManager();

            // let viewportManager know of the viewport
            this.viewportManager.viewport = this.viewport;

            // make sure the OpenGL control is shown (so the OpenGL context is created)
            this.SelectTab(Tabs.Output3D);
            this.SelectTab(Tabs.Code);

            // load XML configuration
            Config.Load();

            // make sure the parameter property grid knows where to look for its items 
            this.parameters.SelectedObject = parameters.Item;

            this.currentFileName = "";

            // ScintillaNet doesn't support the simple TextChanged ecent (not it like the visual event list)
            this.editor.TextInserted += editor_TextInserted;
            this.editor.TextDeleted += editor_TextInserted;

            // trigger the "new file" button click, so the template is loaded (without doubling the code)
            this.newToolStripMenuItem_Click(sender, e);

            // make the output picturebox have zero size (no output is available)
            this.output.Width = 0;
            this.output.Height = 0;

            this.outputManager.ClearData();
            this.outputManager.LoadOverlays();

            // load the custom syntax highlighter settings
            this.editor.ConfigurationManager.CustomLocation = this.config.ScintillaDefinitonsFile;

            // open last opened file if requested
            if (this.config.openLastFileOnStartup && this.config.lastFile != "" && System.IO.File.Exists(this.config.lastFile))
            {
                this.editor.Text = System.IO.File.ReadAllText(this.config.lastFile);
                this.saveFile.FileName = this.config.lastFile;
                this.currentFileName = this.config.lastFile;
                this.needsSaving = false;
            }

            // output and viewport zooming event
            this.MouseWheel += new MouseEventHandler(Form1_MouseWheel);

            // show this form and close the splash screen
            this.Opacity = 1.0;
            //this.loader.Invoke(new MethodInvoker(delegate { this.loader.FadeOut(); }));
            loading.FadeOut();
        }

        // this app's GetInstance()
        public static Main Get()
        {
            try
            {
                /*Form form = System.Windows.Forms.Application.OpenForms[0];

                if (form.GetType() == typeof(Main))
                {
                    return (Main)form;
                }
                else return (Main) System.Windows.Forms.Application.OpenForms[1];*/

                return (Main)System.Windows.Forms.Application.OpenForms[0];
            }
            catch (ArgumentOutOfRangeException)
            {
                Application.Exit();
            }

            return null;
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

        public ViewportManager GetViewportManager()
        {
            return this.viewportManager;
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
                            if (this.currentFileName != "")
                            {
                                System.IO.File.WriteAllText(this.currentFileName, this.editor.Text);
                                this.openFile.FileName = this.currentFileName;
                                this.saveFile.FileName = this.currentFileName;

                                break;
                            }

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

        public void ShowBuildingModel()
        {
            this.AddStatus("Building terrain");

            this.buildingModel.Left = (this.buildingModel.Parent.Width - this.buildingModel.Width) / 2;
            this.buildingModel.Top = (this.buildingModel.Parent.Height - this.buildingModel.Height) / 2;
            this.buildingModel.Visible = true;
        }

        public void HideBuildingModel()
        {
            this.RemoveStatus("Building terrain");
            this.buildingModel.Visible = false;
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
            this.resetToolStripButton.Enabled = true;
        }

        public void OutputButtonsOff()
        {
            this.saveOutput.Enabled = false;
            this.clearOutputs.Enabled = false;
            this.outputs.Enabled = false;
            this.overlays.Enabled = false;
            this.toggleOverlay.Enabled = false;
            this.resetToolStripButton.Enabled = false;
        }

        public void Output3dButtonsOn()
        {
            this.screenshot.Enabled = true;
            this.clear3d.Enabled = true;
            this.outputs3d.Enabled = true;
            this.texture.Enabled = true;
            this.wireframe.Enabled = true;
            //this.lighting.Enabled = true;
            this.heightScale.Enabled = true;
        }

        public void Output3dButtonsOff()
        {
            this.screenshot.Enabled = false;
            this.clear3d.Enabled = false;
            this.outputs3d.Enabled = false;
            this.texture.Enabled = false;
            this.wireframe.Enabled = false;
            //this.lighting.Enabled = false;
            this.heightScale.Enabled = false;
        }


        public void SelectTab(Tabs tab)
        {
            this.tabs.SelectedIndex = (int)tab;
        }

        public void SetStatus(string str)
        {
            this.status.Text = str;
            System.Threading.Thread.Sleep(0);
        }

        public void AddStatus(string status){
            if(!this.statuses.Contains(status)){
                this.statuses.Add(status);
            }

            this.UpdateStatus();
        }

        public void RemoveStatus(string status)
        {
            if (this.statuses.Contains(status))
            {
                this.statuses.Remove(status);
            }

            this.UpdateStatus();
        }

        public void UpdateStatus()
        {
            string newstatus = String.Join(", ", statuses.ToArray());

            if (newstatus == "") this.SetStatus("Ready");
            else this.SetStatus(newstatus);
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

        public void SetSidebarMode(SidebarMode mode)
        {
            this.sidebarMode = mode;

            if (mode == SidebarMode.Left)
            {
                this.leftToolStripMenuItem.Checked = true;
                this.leftToolStripMenuItem.Checked = true;
                this.rightToolStripMenuItem.Checked = false;
                this.bottomToolStripMenuItem.Checked = false;

                this.splitContainer.Orientation = Orientation.Vertical;
                this.splitContainer.RightToLeft = RightToLeft.Yes;
                this.sidebarSplitContainer.Orientation = Orientation.Horizontal;
            }
            else if (mode == SidebarMode.Right)
            {
                this.leftToolStripMenuItem.Checked = false;
                this.rightToolStripMenuItem.Checked = true;
                this.bottomToolStripMenuItem.Checked = false;

                this.splitContainer.Orientation = Orientation.Vertical;
                this.splitContainer.RightToLeft = RightToLeft.No;
                this.sidebarSplitContainer.Orientation = Orientation.Horizontal;
            }
            else
            {
                this.leftToolStripMenuItem.Checked = false;
                this.rightToolStripMenuItem.Checked = false;
                this.bottomToolStripMenuItem.Checked = true;

                this.splitContainer.Orientation = Orientation.Horizontal;
                this.splitContainer.RightToLeft = RightToLeft.No;
                this.sidebarSplitContainer.Orientation = Orientation.Vertical;
            }
        }

        public void LoadInterfaceSettings()
        {
            this.splitContainer.SplitterDistance = this.config.mainSplitter;
            this.sidebarSplitContainer.SplitterDistance = this.config.sidebarSplitter;
            this.SetSidebarMode(this.config.sidebarMode);
            this.statusbarToolStripMenuItem.Checked = this.config.showStatusbar;
            this.sidebarToolStripMenuItem.Checked = this.config.showSidebar;
            this.toolbarToolStripMenuItem.Checked = this.config.showToolbar;
            this.consoleToolStripMenuItem.Checked = this.config.showConsole;
            this.scriptParametersToolStripMenuItem.Checked = this.config.showParameters;
            this.wordWrapToolStripMenuItem.Checked = this.config.wordWrap;
            this.lineBreaksToolStripMenuItem.Checked = this.config.lineBreaks;
            this.whiteSpaceToolStripMenuItem.Checked = this.config.whitespace;
            this.editor.Zoom = this.config.editorZooom;
            this.wireframe.Checked = this.config.wireframe;
            this.heightScale.Value = this.config.heightScale;

            this.statusbarToolStripMenuItem_Click(null, null);
            this.sidebarToolStripMenuItem_Click(null, null);
            this.toolbarToolStripMenuItem_Click(null, null);
            this.consoleToolStripMenuItem_Click(null, null);
            this.scriptParametersToolStripMenuItem_Click(null, null);
            this.wordWrapToolStripMenuItem_Click(null, null);
            this.lineBreaksToolStripMenuItem_Click(null, null);
            this.whiteSpaceToolStripMenuItem_Click(null, null);
            this.heightScale_ValueChanged(null, null);
        }

        public void SaveInterfaceSettings()
        {
            this.config.mainSplitter = this.splitContainer.SplitterDistance;
            this.config.sidebarSplitter = this.sidebarSplitContainer.SplitterDistance;
            this.config.sidebarMode = this.sidebarMode;
            this.config.showStatusbar = this.statusbarToolStripMenuItem.Checked;
            this.config.showSidebar = this.sidebarToolStripMenuItem.Checked;
            this.config.showToolbar = this.toolbarToolStripMenuItem.Checked;
            this.config.showConsole = this.consoleToolStripMenuItem.Checked;
            this.config.showParameters = this.scriptParametersToolStripMenuItem.Checked;
            this.config.wordWrap = this.wordWrapToolStripMenuItem.Checked;
            this.config.lineBreaks = this.lineBreaksToolStripMenuItem.Checked;
            this.config.whitespace = this.whiteSpaceToolStripMenuItem.Checked;
            this.config.editorZooom = this.editor.Zoom;
            this.config.lastFile = this.currentFileName;
            this.config.wireframe = this.wireframe.Checked;
            this.config.heightScale = this.heightScale.Value;

            this.viewportManager.SetupViewport();
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
                this.mouseDownX = e.X;
                this.mouseDownY = e.Y;
            }

            this.output.Focus();
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
                this.output.Left = Math.Min(Math.Max(this.output.Left + (e.X - this.mouseDownX), -this.output.Width + 20), this.outputContainer.Width - 20);
                this.output.Top = Math.Min(Math.Max(this.output.Top + (e.Y - this.mouseDownY), -this.output.Height + 20), this.outputContainer.Height - 20);

                this.output.Update();
                this.outputContainer.Update();
            }

            this.outputLastMouseX = e.X;
            this.outputLastMouseY = e.Y;

            System.Drawing.Bitmap bitmap = (System.Drawing.Bitmap)output.Image;

            if (e.X > 0 && e.Y > 0 && e.X < output.Width && e.Y < output.Height) this.coords.Text = (e.X * output.Image.Width / output.Width) + " x " + (e.Y * output.Image.Height / output.Height) + " [" + bitmap.GetPixel((e.X * output.Image.Width / output.Width), (e.Y * output.Image.Height / output.Height)).R + "]";
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
            this.SetSidebarMode(SidebarMode.Left);
        }

        private void rightToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.SetSidebarMode(SidebarMode.Right);
        }

        private void bottomToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.SetSidebarMode(SidebarMode.Bottom);
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

        private void elementHost1_ChildChanged(object sender, System.Windows.Forms.Integration.ChildChangedEventArgs e)
        {

        }

        private void websiteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("http://code.google.com/p/geogen/");
        }

        private void printToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.editor.Printing.Print(true);
        }

        private void previewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.editor.Printing.PrintPreview();
        }

        private void Form1_MouseWheel(object sender, MouseEventArgs e)
        {
            // mouse is inside the output rect...
            if (this.outputMouse)
            {
                int current_width = output.Width;
                int current_height = output.Height;

                // wheel up/down
                if (e.Delta > 0 && this.output.Width / this.output.Image.Width < 8)
                {
                    output.Width = (Int32)((Double)output.Width * 1.25);
                    output.Height = (Int32)((Double)output.Height * 1.25);
                }
                else if (e.Delta < 0 && this.output.Image.Width / this.output.Width < 16)
                {
                    output.Width = (Int32)((Double)output.Width * 0.8);
                    output.Height = (Int32)((Double)output.Height * 0.8);
                }
                else return;

                // make sure the zooming is centered on mouse
                output.Left -= (Int32)((Double)(output.Width - current_width) / ((Double)current_width / (Double)this.outputLastMouseX));
                output.Top -= (Int32)((Double)(output.Height - current_height) / ((Double)current_height / (Double)this.outputLastMouseY));
            }
            else if(this.viewportMouse){
                this.viewportManager.distance = Math.Max(Math.Min(this.viewportManager.distance - e.Delta / 20, 150), 1);

                this.viewport.Invalidate();
            }
        }

        private void output_MouseEnter(object sender, EventArgs e)
        {
            this.outputMouse = true;
        }

        private void output_MouseLeave(object sender, EventArgs e)
        {
            this.outputMouse = false;
        }

        private void resetToolStripButton_Click(object sender, EventArgs e)
        {
            this.output.Width = this.output.Image.Width;
            this.output.Height = this.output.Image.Height;
            this.output.Left = 0;
            this.output.Top = 0;
        }

        private void viewport_Load(object sender, EventArgs e)
        {
            this.viewportManager.Init();
        }

        private void viewport_Paint(object sender, PaintEventArgs e)
        {
            this.viewportManager.Render();
        }

        private void viewport_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                this.scrollViewport = true;
                this.mouseDownX = e.X;
                this.mouseDownY = e.Y;
            }

            if (e.Button == System.Windows.Forms.MouseButtons.Right)
            {
                this.moveViewport = true;
                this.mouseDownX = e.X;
                this.mouseDownY = e.Y;
            }
        }

        private void viewport_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                this.scrollViewport = false;
            }

            if (e.Button == System.Windows.Forms.MouseButtons.Right)
            {
                this.moveViewport = false;
            }
        }

        private void viewport_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.scrollViewport)
            {
                this.viewportManager.azimuth += (double) (e.X - this.mouseDownX) / (this.viewport.Width * 0.5) ;

                this.viewportManager.elevation = Math.Min(Math.Max(this.viewportManager.elevation + (double)(e.Y - this.mouseDownY) / (this.viewport.Height), 0.174), 1.57); // clamp the elevation between 10 and 90 degrees

                viewport.Invalidate();

            }

            if (this.moveViewport)
            {
                // movement must be relative to the current camera vector (its azimuth)
                // the camerra target is not permitted to leave map area
                this.viewportManager.targetX = Math.Max(0, Math.Min(100 ,this.viewportManager.targetX + ((float)Math.Cos(this.viewportManager.azimuth) * (e.X - this.mouseDownX) / 20 + (float)Math.Sin(-this.viewportManager.azimuth) * (e.Y - this.mouseDownY) / 20)));
                this.viewportManager.targetY = Math.Max(0, Math.Min(100 ,this.viewportManager.targetY - ((float)Math.Cos(-this.viewportManager.azimuth) * (e.Y - this.mouseDownY) / 20 + (float)Math.Sin(this.viewportManager.azimuth) * (e.X - this.mouseDownX) / 20)));

                viewport.Invalidate();
            }


            this.mouseDownX = e.X;
            this.mouseDownY = e.Y;
        }

        private void wireframe_CheckedChanged(object sender, EventArgs e)
        {
            this.viewportManager.SetWireframeState(this.wireframe.Checked);
        }

        private void screenshot_Click(object sender, EventArgs e)
        {
            this.viewportManager.SaveScreenshot();
        }

        private void heightScale_ValueChanged(object sender, EventArgs e)
        {
            this.viewportManager.HeightScale = this.heightScale.Value;
        }

        private void viewport_MouseEnter(object sender, EventArgs e)
        {
            this.viewportMouse = true;
        }

        private void viewport_MouseLeave(object sender, EventArgs e)
        {
            this.viewportMouse = false;
        }

        private void clear3d_Click(object sender, EventArgs e)
        {
            this.viewportManager.ClearData();
        }

        private void outputs3d_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(this.GetViewportManager().currentMap != -1 && this.viewportManager.heightData != null) this.viewportManager.RebuildTerrain();
        }

        private void texture_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.GetViewportManager().ApplyTexture();
        }

        private void lighting_Click(object sender, EventArgs e)
        {
            LightSettings lightSettings = new LightSettings();
            lightSettings.ShowDialog();
        }

        private void viewport_Resize(object sender, EventArgs e)
        {
            this.viewportManager.SetupViewport();
        }


          
    }
}
