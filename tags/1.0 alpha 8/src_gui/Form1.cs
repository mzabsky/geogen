/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    GeoGen is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GeoGen.  If not, see <http://www.gnu.org/licenses/>.

*/

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

        public class GGenCompletionData : ICSharpCode.AvalonEdit.CodeCompletion.ICompletionData
        {
            public GGenCompletionData(string text)
            {
                this.Text = text;
            }

            public System.Windows.Media.ImageSource Image
            {
                get { return null; }
            }

            public string Text { get; private set; }

            // Use this property if you want to show a fancy UIElement in the list.
            public object Content
            {
                get { return this.Text; }
            }

            public double Priority
            {
                get { return 0; }
            }

            public object Description
            {
                get { return "Description for " + this.Text; }
            }

            public void Complete(ICSharpCode.AvalonEdit.Editing.TextArea textArea, ICSharpCode.AvalonEdit.Document.ISegment completionSegment,
                EventArgs insertionRequestEventArgs)
            {
                textArea.Document.Replace(completionSegment, this.Text);
            }
        }

        public ICSharpCode.AvalonEdit.TextEditor editor;
        private bool needsSaving;
        public Config config;
        public string fileFromShell; // file being loaded via command line argument
        private bool knownFile = false; // do we know path to currently edited file (Save action depends on it - it triggers Save As if the file path is not known)
        private bool scrollOutput;
        private bool scrollViewport; // viewport azimuth and elevation mode (left mouse is clicked down)
        private bool moveViewport; // viewport target movement mode (right mouse is clicked down)
        int outputLastMouseX;
        int outputLastMouseY;
        int mouseDownX;
        int mouseDownY;
        bool outputMouse;
        bool viewportMouse;
        SidebarMode sidebarMode = SidebarMode.Right;
        List<string> statuses = new List<string>();
        public string editorBackup = "";
        ICSharpCode.AvalonEdit.Folding.AbstractFoldingStrategy foldingStrategy;
        ICSharpCode.AvalonEdit.Folding.FoldingManager foldingManager;
        public Search searchWindow = new Search();

        public Loading loading = null;

        // MSVS generated stuff
        public Main()
        {
            InitializeComponent();
        }

        // form initializer
        private void Main_Load(object sender, EventArgs e)
        {
            // show the splash screen
            this.loading = new Loading();
            this.loading.Show();
            this.loading.Refresh();

            // set the current directory to the executable dir (staring via shortcut or through associated file could set the working dir to a wrong one)
            System.IO.Directory.SetCurrentDirectory(System.IO.Path.GetDirectoryName(Application.ExecutablePath));

            this.CreateEditor();

            this.needsSaving = false;

            this.currentOverlayIndex = 0;
         
            // load XML configuration
            Config.Load();

            this.InitializeGGen();

            // make sure the OpenGL control is shown (so the OpenGL context is created)
            this.SelectTab(Tabs.Output3D);
            this.SelectTab(Tabs.Code);

            // is OGL ready to rock?
            if (this.viewport.Context == null)
            {
                System.Windows.Forms.MessageBox.Show("Failed to initialize OpenGL." + Environment.NewLine + Environment.NewLine + "The 3D View function will not be available while using this application. If you are sure you have working graphics acceleration with OpenGL 1.4 support, please contact me with details of your system on email address found in the Help -> About GeoGen Studio dialog.");
            }

            // make sure the parameter property grid knows where to look for its items 
            this.parameters.SelectedObject = parameters.Item;

            // ScintillaNet doesn't support the simple TextChanged ecent (not it like the visual event list)
            //this.editor.TextInserted += editor_TextInserted;
            //this.editor.TextDeleted += editor_TextInserted;

            

            // make the output picturebox have zero size (no output is available)
            this.output.Width = 0;
            this.output.Height = 0;

            this.LoadOverlays();

            // open last opened file if requested
            string ext = this.fileFromShell == null ? "" : this.fileFromShell.Substring(this.fileFromShell.LastIndexOf('.'), this.fileFromShell.Length - this.fileFromShell.LastIndexOf('.')).ToLower();
            
            if (this.fileFromShell != null && !(ext == ".shd" || ext == ".bmp" || ext == ".png" || ext == ".jpg"))
            {
                this.editor.Text = System.IO.File.ReadAllText(this.fileFromShell);
                this.knownFile = true;
                this.needsSaving = false;
                this.config.lastFile = this.fileFromShell;
                this.fileFromShell = null; // the file was already loaded and will not be useful any more
            }
            else if (this.config.openLastFileOnStartup && this.config.lastFile != "" && System.IO.File.Exists(this.config.lastFile))
            {              
                this.editor.Text = System.IO.File.ReadAllText(this.config.lastFile);
                this.knownFile = true;
                this.needsSaving = false;
            }
            else
            {
                // trigger the "new file" button click, so the template is loaded (without doubling the code)
                this.newToolStripMenuItem_Click(sender, e);
            }

            // output and viewport zooming event
            this.MouseWheel += new MouseEventHandler(Form1_MouseWheel);

            this.config.Save();

            this.ScheduleSyntaxCheck();



            System.Xml.Serialization.XmlSerializer xs = new System.Xml.Serialization.XmlSerializer(typeof(GGenAPIData));            

            try
            {
                System.IO.StreamReader reader = System.IO.File.OpenText(Program.BasePath + "/config/apidata.xml");

                GGenAPIData apiData = (GGenAPIData) xs.Deserialize(reader);

                foreach(ICSharpCode.AvalonEdit.CodeCompletion.ICompletionData item in apiData.constants){
                    this.completionData.Add(item);
                }

                foreach (ICSharpCode.AvalonEdit.CodeCompletion.ICompletionData item in apiData.methods)
                {
                    this.completionData.Add(item);
                }

                this.completionData.Sort(new CompletionDataComparer());
            }
            catch(Exception ex){
                this.WriteToConsole("Could not load code completion data");
            }

            // fill in the search window (possibly from the data loaded from the config file)
            this.searchWindow.textBox1.Text = this.config.searchString;
            this.searchWindow.textBox2.Text = this.config.replaceString;
            this.searchWindow.matchCase.Checked = this.config.searchMode == StringComparison.Ordinal;

            // show this form and close the splash screen
            this.Opacity = 1.0;
            
            this.loading.FadeOut();

            if(this.fileFromShell != null){
                this.ClearData();
                this.ClearData3D();
                this.ReloadMaps(this.fileFromShell);
            }
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

        public void EditorTextChanged(){
            this.needsSaving = true;
            this.foldingStrategy.UpdateFoldings(this.foldingManager, this.editor.Document);
            this.ScheduleSyntaxCheck();
        }

        public void CreateEditor(){
            this.editor = new ICSharpCode.AvalonEdit.TextEditor();

            using (System.Xml.XmlTextReader reader = new System.Xml.XmlTextReader(Program.BasePath + "config/squirrel.xshd"))
            {
                try
                {
                    this.editor.SyntaxHighlighting = ICSharpCode.AvalonEdit.Highlighting.Xshd.HighlightingLoader.Load(reader, ICSharpCode.AvalonEdit.Highlighting.HighlightingManager.Instance);
                }
                catch (System.IO.FileNotFoundException)
                {
                    this.WriteToConsole("Could not read syntax highlighter definitions file.");
                }
            }

            editor.TextChanged += delegate(object s, System.EventArgs args)
            {
                this.EditorTextChanged();
            };

            editor.TextArea.TextEntered += delegate(object s, System.Windows.Input.TextCompositionEventArgs args)
            {
                this.EditorTextChanged();
            };

            this.RegisterCompletionEvents();

            this.editor.FontFamily = new System.Windows.Media.FontFamily("Consolas, Courier New");
            this.editor.WordWrap = false;
            this.editor.ShowLineNumbers = true;
            this.editor.FontSize = 13;


            // code folding
            foldingManager = ICSharpCode.AvalonEdit.Folding.FoldingManager.Install(editor.TextArea);
            foldingStrategy = new AvalonEdit.Sample.BraceFoldingStrategy();
            this.foldingStrategy.UpdateFoldings(this.foldingManager, this.editor.Document);

            this.editorHost.Child = this.editor;
        }

        public Config GetConfig()
        {
            return this.config;
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

        public bool FileDialog(FileDialog dialog, ref string path)
        {
            try
            {
                System.IO.FileInfo info = new System.IO.FileInfo(path);
                dialog.InitialDirectory = info.DirectoryName;
                dialog.FileName = info.Name;
            }
            catch { };

            if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                path = dialog.FileName;

                return true;
            }

            return false;
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
                            if (this.knownFile)
                            {
                                System.IO.File.WriteAllText(this.config.lastFile, this.editor.Text);

                                break;
                            }

                            if (this.FileDialog(saveFile, ref this.config.lastFile))
                            {
                                System.IO.File.WriteAllText(this.config.lastFile, this.editor.Text);

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
            // is this a cross-thread call?
            if (this.InvokeRequired)
            {
                this.Invoke(new MethodInvoker(delegate()
                {
                    this.console.Text += str + Environment.NewLine;
                }));
            }
            else this.console.Text += str + Environment.NewLine;
        }

        public void Error(string error)
        {
            System.Windows.Forms.MessageBox.Show(error);
        }

        public void OutOfMemory()
        {
            try
            {
                this.ClearData();
                //this.outputManager.ClearData();
            }
            catch { };

            this.Error("GeoGen Studio ran out of memory, 2D and 3D data had to be released in order to prevent application crash.");
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
            this.importtoolStripMenuItem.Enabled = false;
            this.progress.Visible = true;
        }

        public void ButtonsNoRunMode()
        {
            this.executeToolStripButton.Enabled = true;
            this.executeToolStripMenuItem.Enabled = true;
            this.terminateToolStripButton.Enabled = false;
            this.terminateToolStripMenuItem.Enabled = false;
            this.benchmarkToolStripButton.Enabled = true;
            this.benchmarkToolStripMenuItem.Enabled = true;
            this.importtoolStripMenuItem.Enabled = true;
            this.progress.Visible = false;
        }

        public void OutputButtonsOn()
        {
            this.saveOutput.Enabled = true;
            //this.clearOutputs.Enabled = true;
            this.outputs.Enabled = true;
            this.overlays.Enabled = true;
            this.toggleOverlay.Enabled = true;
            this.resetToolStripButton.Enabled = true;
            this.exportToolStripMenuItem.Enabled = true;
            this.exportToolStripButton.Enabled = true;
            this.output.Visible = true;
        }

        public void OutputButtonsOff()
        {
            this.saveOutput.Enabled = false;
            //this.clearOutputs.Enabled = false;
            this.outputs.Enabled = false;
            this.overlays.Enabled = false;
            this.toggleOverlay.Enabled = false;
            this.resetToolStripButton.Enabled = false;
            this.exportToolStripMenuItem.Enabled = false;
            this.exportToolStripButton.Enabled = false;
            this.output.Visible = false;
        }

        public void Output3dButtonsOn()
        {
            this.screenshot.Enabled = true;
            //this.clear3d.Enabled = true;
            this.outputs3d.Enabled = true;
            this.texture.Enabled = true;
            this.wireframe.Enabled = true;
            //this.lighting.Enabled = true;
            this.heightScale.Enabled = true;

            this.ready3d = true;
        }

        public void Output3dButtonsOff()
        {
            this.screenshot.Enabled = false;
            //this.clear3d.Enabled = false;
            this.outputs3d.Enabled = false;
            this.texture.Enabled = false;
            this.wireframe.Enabled = false;
            //this.lighting.Enabled = false;
            this.heightScale.Enabled = false;

            //viewport.Invalidate();

            this.ready3d = false;
        }


        public void SelectTab(Tabs tab)
        {
            this.tabs.SelectedIndex = (int)tab;
        }

        public void SetStatus(string str)
        {
            this.Invoke(
                new MethodInvoker(delegate()
                {
                    this.status.Text = str;
                    System.Threading.Thread.Sleep(0);
                }
             ));
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
            this.Width = this.config.windowWidth;
            this.Height = this.config.windowHeight;
            this.WindowState = this.config.windowState;
            this.splitContainer.SplitterDistance = this.config.mainSplitter;
            this.sidebarSplitContainer.SplitterDistance = this.config.sidebarSplitter;
            this.SetSidebarMode(this.config.sidebarMode);
            this.statusbarToolStripMenuItem.Checked = this.config.showStatusbar;
            this.sidebarToolStripMenuItem.Checked = this.config.showSidebar;
            this.toolbarToolStripMenuItem.Checked = this.config.showToolbar;
            this.consoleToolStripMenuItem.Checked = this.config.showConsole;
            this.scriptParametersToolStripMenuItem.Checked = this.config.showParameters;
            this.wordWrapToolStripMenuItem.Checked = this.config.wordWrap;
            this.editor.FontSize = this.config.fontSize;
            this.wireframe.Checked = this.config.wireframe;
            this.heightScale.Value = this.config.heightScale;
            this.importHeightmapDialog.FileName = this.config.lastImportedFile;
            this.exportHeightmapDialog.FileName = this.config.lastExportedFile;

            this.wireframe.Checked = this.config.wireframe;
            this.oldValues = this.config.parameters;

            this.statusbarToolStripMenuItem_Click(null, null);
            this.sidebarToolStripMenuItem_Click(null, null);
            this.toolbarToolStripMenuItem_Click(null, null);
            this.consoleToolStripMenuItem_Click(null, null);
            this.scriptParametersToolStripMenuItem_Click(null, null);
            this.wordWrapToolStripMenuItem_Click(null, null);
            this.heightScale_ValueChanged(null, null);
        }

        public void SaveInterfaceSettings()
        {
            this.config.windowWidth = this.Width;
            this.config.windowHeight = this.Height;
            this.config.windowState = this.WindowState;
            this.config.mainSplitter = this.splitContainer.SplitterDistance;
            this.config.sidebarSplitter = this.sidebarSplitContainer.SplitterDistance;
            this.config.sidebarMode = this.sidebarMode;
            this.config.showStatusbar = this.statusbarToolStripMenuItem.Checked;
            this.config.showSidebar = this.sidebarToolStripMenuItem.Checked;
            this.config.showToolbar = this.toolbarToolStripMenuItem.Checked;
            this.config.showConsole = this.consoleToolStripMenuItem.Checked;
            this.config.showParameters = this.scriptParametersToolStripMenuItem.Checked;
            this.config.wordWrap = this.wordWrapToolStripMenuItem.Checked;
            this.config.fontSize = (uint) this.editor.FontSize;
            this.config.wireframe = this.wireframe.Checked;
            this.config.heightScale = this.heightScale.Value;

            this.config.parameters = this.GetArgValues();

            this.SetupViewport();
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
            //this.editor.UndoRedo.EmptyUndoBuffer();

            // make sure the file won't overwrite the previously edited file when hitting Ctrl + S
            this.knownFile = false;

            this.needsSaving = false;
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // save the file if requested
            if (!this.TryToSaveIfUnsaved()) return;

            if (this.FileDialog(openFile, ref this.config.lastFile))
            {
                this.editor.Text = System.IO.File.ReadAllText(this.config.lastFile);
                this.knownFile = true;
                this.needsSaving = false;

                //this.editor.UndoRedo.EmptyUndoBuffer();
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // trigger the Save As action if the current file is not known
            if (!this.knownFile)
            {
                this.saveAsToolStripMenuItem_Click(sender, e);
            }
            else
            {
                System.IO.File.WriteAllText(this.config.lastFile, this.editor.Text);

                this.needsSaving = false;
            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.FileDialog(saveFile, ref this.config.lastFile))
            {
                System.IO.File.WriteAllText(this.config.lastFile, this.editor.Text);
                this.knownFile = true;
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

        /*private void editor_TextInserted(object sender, ScintillaNet.TextModifiedEventArgs e)
        {
            this.ScheduleSyntaxCheck();

            this.needsSaving = true;

            if(this.editor.Text.Length > 0) this.editorBackup = this.editor.Text;
        }*/

        private void executeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //if (this.parameters.Focused)
            //{
                this.editor.Focus();
                //this.parameters.Focus();

            //this.Foc
            //}

            this.ExecuteScript(editor.Text, false, null);
        }

        private void refreshOverlays_Click(object sender, EventArgs e)
        {
            this.LoadOverlays();
        }

        private void toggleOverlay_Click(object sender, EventArgs e)
        {
            this.ToggleOverlay();
        }

        private void overlays_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(this.overlays.SelectedIndex > 0) this.toggleOverlay.Checked = true;
            else this.toggleOverlay.Checked = false;

             this.ShowImage();
        }

        private void outputs_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.ShowImage();
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


                System.Drawing.Bitmap bitmap = (System.Drawing.Bitmap)output.Image;

                if (e.X > 0 && e.Y > 0 && e.X < output.Width && e.Y < output.Height) this.coords.Text = (e.X * output.Image.Width / output.Width) + " x " + (e.Y * output.Image.Height / output.Height) + " [" + bitmap.GetPixel((e.X * output.Image.Width / output.Width), (e.Y * output.Image.Height / output.Height)).R + "]";
            }

            this.outputLastMouseX = e.X;
            this.outputLastMouseY = e.Y;
        }

        private void terminateToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Terminate();
        }

        private void verifyToolStripButton_Click(object sender, EventArgs e)
        {
            this.ShowLastErrorInfo();
        }

        private void clearOutputs_Click(object sender, EventArgs e)
        {
            //this.outputManager.ClearData();

            this.SelectTab(Tabs.Code);
        }

        private void saveOutput_Click(object sender, EventArgs e)
        {
            this.SaveOutput();
        }

        private void benchmarkToolStripButton_Click(object sender, EventArgs e)
        {
            this.StartBenchmark();
        }

        private void undoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.editor.Undo();
        }

        private void redoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.editor.Redo();
        }

        private void cutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.editor.Cut();
        }

        private void copyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.editor.Copy();
        }

        private void pasteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.editor.Paste();
        }

        private void searchAndReplaceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.searchWindow.Show(this);
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
            this.editor.WordWrap = this.wordWrapToolStripMenuItem.Checked;
        }

        private void increaseFontSizeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.editor.FontSize = Math.Min(this.editor.FontSize + 1, 25);
        }

        private void decreaseFontSizeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.editor.FontSize = Math.Max(this.editor.FontSize - 1, 10);
        }

        private void resetFontSizeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.editor.FontSize = this.config.defaultFontSize;
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
            //this.editor.Printing.Print(true);
        }

        private void previewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //this.editor.Printing.PrintPreview();
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
                this.distance = Math.Max(Math.Min(this.distance - e.Delta / 20, 150), 1);

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
            if (this.output.Image == null) return;
            
            this.output.Width = this.output.Image.Width;
            this.output.Height = this.output.Image.Height;
            this.output.Left = 0;
            this.output.Top = 0;
        }

        private void viewport_Load(object sender, EventArgs e)
        {
            this.Init();
        }

        private void viewport_Paint(object sender, PaintEventArgs e)
        {
            this.Render(e);
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
                this.azimuth += (double) (e.X - this.mouseDownX) / (this.viewport.Width * 0.5) ;

                this.elevation = Math.Min(Math.Max(this.elevation + (double)(e.Y - this.mouseDownY) / (this.viewport.Height), 0.174), 1.57); // clamp the elevation between 10 and 90 degrees

                viewport.Invalidate();

            }

            if (this.moveViewport)
            {
                // movement must be relative to the current camera vector (its azimuth)
                // the camerra target is not permitted to leave map area
                this.targetX = Math.Max(0, Math.Min(100 ,this.targetX + ((float)Math.Cos(this.azimuth) * (e.X - this.mouseDownX) / 20 + (float)Math.Sin(-this.azimuth) * (e.Y - this.mouseDownY) / 20)));
                this.targetY = Math.Max(0, Math.Min(100 ,this.targetY - ((float)Math.Cos(-this.azimuth) * (e.Y - this.mouseDownY) / 20 + (float)Math.Sin(this.azimuth) * (e.X - this.mouseDownX) / 20)));

                viewport.Invalidate();
            }


            this.mouseDownX = e.X;
            this.mouseDownY = e.Y;
        }

        private void wireframe_CheckedChanged(object sender, EventArgs e)
        {
            this.SetWireframeState(this.wireframe.Checked);
        }

        private void screenshot_Click(object sender, EventArgs e)
        {
            this.SaveScreenshot();
        }

        private void heightScale_ValueChanged(object sender, EventArgs e)
        {
            this.viewport.Invalidate();
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
            this.ClearData();
        }

        private void outputs3d_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(this.currentMap != -1 && this.heightData != null && (this.outputs3d.SelectedItem as string) != "[Imported File]") this.RebuildTerrain(null);
        }

        private void texture_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.texture.SelectedIndex != -1)
            {
                this.currentTexture = (string) this.texture.Items[this.texture.SelectedIndex];
            }

            this.ApplyTexture();
        }

        private void viewport_Resize(object sender, EventArgs e)
        {
            this.SetupViewport();
        }

        private void Main_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
                e.Effect = DragDropEffects.All;
            else
                e.Effect = DragDropEffects.None;
        }

        private void Main_DragDrop(object sender, DragEventArgs e)
        {
            string file = ((string[])e.Data.GetData(DataFormats.FileDrop))[0];

            string ext = file.Substring(file.LastIndexOf('.'), file.Length - file.LastIndexOf('.')).ToLower() ;
             
            if (ext == ".shd" || ext == ".bmp" || ext == ".png" || ext == ".jpg")
            {
                this.ClearData();
                this.ClearData3D();

                this.ReloadMaps(file);
            }
            else
            {
                this.editor.Text = System.IO.File.ReadAllText(file);
                this.config.lastFile = file;
                this.knownFile = true;
                this.needsSaving = false;
            }
        }

        private void importtoolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.FileDialog(this.importHeightmapDialog, ref this.config.lastImportedFile))
            {
                this.ReloadMaps(this.config.lastImportedFile);
            }
        }

        private void toolStripMenuItem2_Click(object sender, EventArgs e)
        {
            this.ExportData();
        }

        private void exportToolStripButton_Click(object sender, EventArgs e)
        {
            this.ExportData();
        }

        private void viewportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.SelectTab(Tabs.Output3D);
        }

        public void findNextToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            int searchResult = this.editor.Text.IndexOf(this.config.searchString, this.editor.CaretOffset, this.config.searchMode);

            if(searchResult == -1){
                searchResult = this.editor.Text.IndexOf(this.config.searchString, 0, this.config.searchMode);

                if (searchResult == -1)
                {
                    System.Windows.Forms.MessageBox.Show("No occurrences of \"" + this.config.searchString + "\" were found.");
                    return;
                }
            }

            int endOffset = searchResult + this.config.searchString.Length;
            this.editor.TextArea.Caret.Offset = endOffset;
            this.editor.TextArea.Selection = new ICSharpCode.AvalonEdit.Editing.SimpleSelection(searchResult, endOffset);
            this.editor.TextArea.Caret.BringCaretToView();
        }

        public void findPrevToolStripMenuItem_Click(object sender, EventArgs e)
        {
            int searchResult = this.editor.Text.LastIndexOf(this.config.searchString, this.editor.SelectionStart > 0 ? this.editor.SelectionStart : 0, this.config.searchMode);

            if(searchResult == -1){
                searchResult = this.editor.Text.LastIndexOf(this.config.searchString, this.editor.Text.Length - 1, this.config.searchMode);

                if(searchResult == -1){
                    System.Windows.Forms.MessageBox.Show("No occurrences of \"" + this.config.searchString + "\" were found.");
                    return;
                }
            }

            int endOffset = searchResult + this.config.searchString.Length;
            this.editor.TextArea.Caret.Offset = endOffset;
            this.editor.TextArea.Selection = new ICSharpCode.AvalonEdit.Editing.SimpleSelection(searchResult, endOffset);
            this.editor.TextArea.Caret.BringCaretToView();
        }

        public void replaceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            int selectionStart = editor.SelectionStart;

            this.editor.Document.Replace(selectionStart, this.editor.SelectionLength, this.config.replaceString);

            this.editor.CaretOffset = selectionStart + this.config.replaceString.Length;

            this.findNextToolStripMenuItem_Click_1(sender, e);
        }

        public void ReplaceAll()
        {
            int caret = 0;
            while ((caret = this.editor.Text.IndexOf(this.config.searchString, caret, this.config.searchMode)) != -1)
            {
                editor.Document.Replace(caret, this.config.searchString.Length, this.config.replaceString);
            }
        }          
    }
}
