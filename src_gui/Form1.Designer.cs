namespace GeoGen_Studio
{
    partial class Main
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
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.TabPage codeTab;
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Main));
            this.editor = new ScintillaNet.Scintilla();
            this.menuStrip = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.printToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.undoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.redoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator6 = new System.Windows.Forms.ToolStripSeparator();
            this.cutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.copyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.pasteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
            this.searchAndReplaceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator8 = new System.Windows.Forms.ToolStripSeparator();
            this.settingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.codeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.outputToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.logToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.documentationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator9 = new System.Windows.Forms.ToolStripSeparator();
            this.sidebarToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.scriptParametersToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.consoleToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator13 = new System.Windows.Forms.ToolStripSeparator();
            this.leftToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.rightToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.bottomToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolbarToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.statusbarToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator10 = new System.Windows.Forms.ToolStripSeparator();
            this.wordWrapToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator11 = new System.Windows.Forms.ToolStripSeparator();
            this.increaseFontSizeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.decreaseFontSizeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.resetFontSizeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator12 = new System.Windows.Forms.ToolStripSeparator();
            this.whiteSpaceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lineBreaksToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.squirrelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.verifyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.executeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.terminateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.benchmarkToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStrip = new System.Windows.Forms.ToolStrip();
            this.newToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.openToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.saveToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.printToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator = new System.Windows.Forms.ToolStripSeparator();
            this.cutToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.copyToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.pasteToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.verifyToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.executeToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.terminateToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.benchmarkToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.status = new System.Windows.Forms.ToolStripStatusLabel();
            this.coords = new System.Windows.Forms.ToolStripStatusLabel();
            this.splitContainer = new System.Windows.Forms.SplitContainer();
            this.tabs = new System.Windows.Forms.TabControl();
            this.outputTab = new System.Windows.Forms.TabPage();
            this.outputContainer = new System.Windows.Forms.Panel();
            this.output = new System.Windows.Forms.PictureBox();
            this.outputToolStrip = new System.Windows.Forms.ToolStrip();
            this.saveOutput = new System.Windows.Forms.ToolStripButton();
            this.clearOutputs = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.outputs = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripLabel2 = new System.Windows.Forms.ToolStripLabel();
            this.overlays = new System.Windows.Forms.ToolStripComboBox();
            this.toggleOverlay = new System.Windows.Forms.ToolStripButton();
            this.refreshOverlays = new System.Windows.Forms.ToolStripButton();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.consoleLarge = new System.Windows.Forms.TextBox();
            this.sidebarSplitContainer = new System.Windows.Forms.SplitContainer();
            this.parameters = new PropertyGridEx.PropertyGridEx();
            this.console = new System.Windows.Forms.TextBox();
            this.saveFile = new System.Windows.Forms.SaveFileDialog();
            this.openFile = new System.Windows.Forms.OpenFileDialog();
            this.saveOutputDialog = new System.Windows.Forms.SaveFileDialog();
            codeTab = new System.Windows.Forms.TabPage();
            codeTab.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.editor)).BeginInit();
            this.menuStrip.SuspendLayout();
            this.toolStrip.SuspendLayout();
            this.statusStrip.SuspendLayout();
            this.splitContainer.Panel1.SuspendLayout();
            this.splitContainer.Panel2.SuspendLayout();
            this.splitContainer.SuspendLayout();
            this.tabs.SuspendLayout();
            this.outputTab.SuspendLayout();
            this.outputContainer.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.output)).BeginInit();
            this.outputToolStrip.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.sidebarSplitContainer.Panel1.SuspendLayout();
            this.sidebarSplitContainer.Panel2.SuspendLayout();
            this.sidebarSplitContainer.SuspendLayout();
            this.SuspendLayout();
            // 
            // codeTab
            // 
            codeTab.Controls.Add(this.editor);
            codeTab.Location = new System.Drawing.Point(4, 22);
            codeTab.Name = "codeTab";
            codeTab.Padding = new System.Windows.Forms.Padding(3);
            codeTab.Size = new System.Drawing.Size(910, 541);
            codeTab.TabIndex = 0;
            codeTab.Text = "Code";
            codeTab.UseVisualStyleBackColor = true;
            // 
            // editor
            // 
            this.editor.CallTip.BackColor = System.Drawing.SystemColors.Window;
            this.editor.Caret.BlinkRate = 500;
            this.editor.ConfigurationManager.CustomLocation = "../config/scintilla.xml";
            this.editor.ConfigurationManager.IsBuiltInEnabled = false;
            this.editor.ConfigurationManager.IsUserEnabled = false;
            this.editor.ConfigurationManager.Language = "cs";
            this.editor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.editor.Location = new System.Drawing.Point(3, 3);
            this.editor.Margins.FoldMarginColor = System.Drawing.Color.Gray;
            this.editor.Margins.FoldMarginHighlightColor = System.Drawing.Color.LightGray;
            this.editor.Margins.Margin0.Width = 30;
            this.editor.Margins.Margin2.IsClickable = false;
            this.editor.Margins.Margin2.IsFoldMargin = false;
            this.editor.Name = "editor";
            this.editor.Size = new System.Drawing.Size(904, 535);
            this.editor.Styles.Bits = 5;
            this.editor.Styles.BraceBad.BackColor = System.Drawing.SystemColors.Window;
            this.editor.Styles.BraceLight.BackColor = System.Drawing.SystemColors.Window;
            this.editor.Styles.CallTip.BackColor = System.Drawing.SystemColors.Window;
            this.editor.Styles.ControlChar.BackColor = System.Drawing.SystemColors.Window;
            this.editor.Styles.Default.BackColor = System.Drawing.SystemColors.Window;
            this.editor.Styles.IndentGuide.BackColor = System.Drawing.SystemColors.Window;
            this.editor.Styles.LastPredefined.BackColor = System.Drawing.SystemColors.Window;
            this.editor.Styles.Max.BackColor = System.Drawing.SystemColors.Window;
            this.editor.TabIndex = 0;
            // 
            // menuStrip
            // 
            this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.viewToolStripMenuItem,
            this.squirrelToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip.Location = new System.Drawing.Point(0, 0);
            this.menuStrip.Name = "menuStrip";
            this.menuStrip.Size = new System.Drawing.Size(1139, 24);
            this.menuStrip.TabIndex = 0;
            this.menuStrip.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.openToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.saveAsToolStripMenuItem,
            this.toolStripSeparator4,
            this.printToolStripMenuItem,
            this.toolStripSeparator5,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("newToolStripMenuItem.Image")));
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
            this.newToolStripMenuItem.Size = new System.Drawing.Size(195, 22);
            this.newToolStripMenuItem.Text = "New";
            this.newToolStripMenuItem.Click += new System.EventHandler(this.newToolStripMenuItem_Click);
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("openToolStripMenuItem.Image")));
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.openToolStripMenuItem.Size = new System.Drawing.Size(195, 22);
            this.openToolStripMenuItem.Text = "Open";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("saveToolStripMenuItem.Image")));
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(195, 22);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift)
                        | System.Windows.Forms.Keys.S)));
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(195, 22);
            this.saveAsToolStripMenuItem.Text = "Save As...";
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.saveAsToolStripMenuItem_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(192, 6);
            // 
            // printToolStripMenuItem
            // 
            this.printToolStripMenuItem.Enabled = false;
            this.printToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("printToolStripMenuItem.Image")));
            this.printToolStripMenuItem.Name = "printToolStripMenuItem";
            this.printToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.P)));
            this.printToolStripMenuItem.Size = new System.Drawing.Size(195, 22);
            this.printToolStripMenuItem.Text = "Print";
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(192, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.F4)));
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(195, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.undoToolStripMenuItem,
            this.redoToolStripMenuItem,
            this.toolStripSeparator6,
            this.cutToolStripMenuItem,
            this.copyToolStripMenuItem,
            this.pasteToolStripMenuItem,
            this.toolStripSeparator7,
            this.searchAndReplaceToolStripMenuItem,
            this.toolStripSeparator8,
            this.settingsToolStripMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(39, 20);
            this.editToolStripMenuItem.Text = "Edit";
            // 
            // undoToolStripMenuItem
            // 
            this.undoToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("undoToolStripMenuItem.Image")));
            this.undoToolStripMenuItem.Name = "undoToolStripMenuItem";
            this.undoToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Z)));
            this.undoToolStripMenuItem.Size = new System.Drawing.Size(216, 22);
            this.undoToolStripMenuItem.Text = "Undo";
            this.undoToolStripMenuItem.Click += new System.EventHandler(this.undoToolStripMenuItem_Click);
            // 
            // redoToolStripMenuItem
            // 
            this.redoToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("redoToolStripMenuItem.Image")));
            this.redoToolStripMenuItem.Name = "redoToolStripMenuItem";
            this.redoToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift)
                        | System.Windows.Forms.Keys.Z)));
            this.redoToolStripMenuItem.Size = new System.Drawing.Size(216, 22);
            this.redoToolStripMenuItem.Text = "Redo";
            this.redoToolStripMenuItem.Click += new System.EventHandler(this.redoToolStripMenuItem_Click);
            // 
            // toolStripSeparator6
            // 
            this.toolStripSeparator6.Name = "toolStripSeparator6";
            this.toolStripSeparator6.Size = new System.Drawing.Size(213, 6);
            // 
            // cutToolStripMenuItem
            // 
            this.cutToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("cutToolStripMenuItem.Image")));
            this.cutToolStripMenuItem.Name = "cutToolStripMenuItem";
            this.cutToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
            this.cutToolStripMenuItem.Size = new System.Drawing.Size(216, 22);
            this.cutToolStripMenuItem.Text = "Cut";
            this.cutToolStripMenuItem.Click += new System.EventHandler(this.cutToolStripMenuItem_Click);
            // 
            // copyToolStripMenuItem
            // 
            this.copyToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("copyToolStripMenuItem.Image")));
            this.copyToolStripMenuItem.Name = "copyToolStripMenuItem";
            this.copyToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
            this.copyToolStripMenuItem.Size = new System.Drawing.Size(216, 22);
            this.copyToolStripMenuItem.Text = "Copy";
            this.copyToolStripMenuItem.Click += new System.EventHandler(this.copyToolStripMenuItem_Click);
            // 
            // pasteToolStripMenuItem
            // 
            this.pasteToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("pasteToolStripMenuItem.Image")));
            this.pasteToolStripMenuItem.Name = "pasteToolStripMenuItem";
            this.pasteToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.V)));
            this.pasteToolStripMenuItem.Size = new System.Drawing.Size(216, 22);
            this.pasteToolStripMenuItem.Text = "Paste";
            this.pasteToolStripMenuItem.Click += new System.EventHandler(this.pasteToolStripMenuItem_Click);
            // 
            // toolStripSeparator7
            // 
            this.toolStripSeparator7.Name = "toolStripSeparator7";
            this.toolStripSeparator7.Size = new System.Drawing.Size(213, 6);
            // 
            // searchAndReplaceToolStripMenuItem
            // 
            this.searchAndReplaceToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("searchAndReplaceToolStripMenuItem.Image")));
            this.searchAndReplaceToolStripMenuItem.Name = "searchAndReplaceToolStripMenuItem";
            this.searchAndReplaceToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.F)));
            this.searchAndReplaceToolStripMenuItem.Size = new System.Drawing.Size(216, 22);
            this.searchAndReplaceToolStripMenuItem.Text = "Search and Replace";
            this.searchAndReplaceToolStripMenuItem.Click += new System.EventHandler(this.searchAndReplaceToolStripMenuItem_Click);
            // 
            // toolStripSeparator8
            // 
            this.toolStripSeparator8.Name = "toolStripSeparator8";
            this.toolStripSeparator8.Size = new System.Drawing.Size(213, 6);
            // 
            // settingsToolStripMenuItem
            // 
            this.settingsToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("settingsToolStripMenuItem.Image")));
            this.settingsToolStripMenuItem.Name = "settingsToolStripMenuItem";
            this.settingsToolStripMenuItem.Size = new System.Drawing.Size(216, 22);
            this.settingsToolStripMenuItem.Text = "Settings";
            this.settingsToolStripMenuItem.Click += new System.EventHandler(this.settingsToolStripMenuItem_Click);
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.codeToolStripMenuItem,
            this.outputToolStripMenuItem,
            this.logToolStripMenuItem,
            this.documentationToolStripMenuItem,
            this.toolStripSeparator9,
            this.sidebarToolStripMenuItem,
            this.toolbarToolStripMenuItem,
            this.statusbarToolStripMenuItem,
            this.toolStripSeparator10,
            this.wordWrapToolStripMenuItem,
            this.toolStripSeparator11,
            this.increaseFontSizeToolStripMenuItem,
            this.decreaseFontSizeToolStripMenuItem,
            this.resetFontSizeToolStripMenuItem,
            this.toolStripSeparator12,
            this.whiteSpaceToolStripMenuItem,
            this.lineBreaksToolStripMenuItem});
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            this.viewToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.viewToolStripMenuItem.Text = "View";
            // 
            // codeToolStripMenuItem
            // 
            this.codeToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("codeToolStripMenuItem.Image")));
            this.codeToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.codeToolStripMenuItem.Name = "codeToolStripMenuItem";
            this.codeToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F7;
            this.codeToolStripMenuItem.Size = new System.Drawing.Size(176, 22);
            this.codeToolStripMenuItem.Text = "Code";
            this.codeToolStripMenuItem.Click += new System.EventHandler(this.codeToolStripMenuItem_Click);
            // 
            // outputToolStripMenuItem
            // 
            this.outputToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("outputToolStripMenuItem.Image")));
            this.outputToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.outputToolStripMenuItem.Name = "outputToolStripMenuItem";
            this.outputToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F8;
            this.outputToolStripMenuItem.Size = new System.Drawing.Size(176, 22);
            this.outputToolStripMenuItem.Text = "Output";
            this.outputToolStripMenuItem.Click += new System.EventHandler(this.outputToolStripMenuItem_Click);
            // 
            // logToolStripMenuItem
            // 
            this.logToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("logToolStripMenuItem.Image")));
            this.logToolStripMenuItem.Name = "logToolStripMenuItem";
            this.logToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F10;
            this.logToolStripMenuItem.Size = new System.Drawing.Size(176, 22);
            this.logToolStripMenuItem.Text = "Log";
            this.logToolStripMenuItem.Click += new System.EventHandler(this.logToolStripMenuItem_Click);
            // 
            // documentationToolStripMenuItem
            // 
            this.documentationToolStripMenuItem.Enabled = false;
            this.documentationToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("documentationToolStripMenuItem.Image")));
            this.documentationToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.documentationToolStripMenuItem.Name = "documentationToolStripMenuItem";
            this.documentationToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F1;
            this.documentationToolStripMenuItem.Size = new System.Drawing.Size(176, 22);
            this.documentationToolStripMenuItem.Text = "Documentation";
            // 
            // toolStripSeparator9
            // 
            this.toolStripSeparator9.Name = "toolStripSeparator9";
            this.toolStripSeparator9.Size = new System.Drawing.Size(173, 6);
            // 
            // sidebarToolStripMenuItem
            // 
            this.sidebarToolStripMenuItem.Checked = true;
            this.sidebarToolStripMenuItem.CheckOnClick = true;
            this.sidebarToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.sidebarToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.scriptParametersToolStripMenuItem,
            this.consoleToolStripMenuItem,
            this.toolStripSeparator13,
            this.leftToolStripMenuItem,
            this.rightToolStripMenuItem,
            this.bottomToolStripMenuItem});
            this.sidebarToolStripMenuItem.Name = "sidebarToolStripMenuItem";
            this.sidebarToolStripMenuItem.Size = new System.Drawing.Size(176, 22);
            this.sidebarToolStripMenuItem.Text = "Sidebar";
            this.sidebarToolStripMenuItem.Click += new System.EventHandler(this.sidebarToolStripMenuItem_Click);
            // 
            // scriptParametersToolStripMenuItem
            // 
            this.scriptParametersToolStripMenuItem.Checked = true;
            this.scriptParametersToolStripMenuItem.CheckOnClick = true;
            this.scriptParametersToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.scriptParametersToolStripMenuItem.Name = "scriptParametersToolStripMenuItem";
            this.scriptParametersToolStripMenuItem.Size = new System.Drawing.Size(166, 22);
            this.scriptParametersToolStripMenuItem.Text = "Script Parameters";
            this.scriptParametersToolStripMenuItem.Click += new System.EventHandler(this.scriptParametersToolStripMenuItem_Click);
            // 
            // consoleToolStripMenuItem
            // 
            this.consoleToolStripMenuItem.Checked = true;
            this.consoleToolStripMenuItem.CheckOnClick = true;
            this.consoleToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.consoleToolStripMenuItem.Name = "consoleToolStripMenuItem";
            this.consoleToolStripMenuItem.Size = new System.Drawing.Size(166, 22);
            this.consoleToolStripMenuItem.Text = "Console";
            this.consoleToolStripMenuItem.Click += new System.EventHandler(this.consoleToolStripMenuItem_Click);
            // 
            // toolStripSeparator13
            // 
            this.toolStripSeparator13.Name = "toolStripSeparator13";
            this.toolStripSeparator13.Size = new System.Drawing.Size(163, 6);
            // 
            // leftToolStripMenuItem
            // 
            this.leftToolStripMenuItem.Name = "leftToolStripMenuItem";
            this.leftToolStripMenuItem.Size = new System.Drawing.Size(166, 22);
            this.leftToolStripMenuItem.Text = "Left";
            this.leftToolStripMenuItem.Click += new System.EventHandler(this.leftToolStripMenuItem_Click);
            // 
            // rightToolStripMenuItem
            // 
            this.rightToolStripMenuItem.Checked = true;
            this.rightToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.rightToolStripMenuItem.Name = "rightToolStripMenuItem";
            this.rightToolStripMenuItem.Size = new System.Drawing.Size(166, 22);
            this.rightToolStripMenuItem.Text = "Right";
            this.rightToolStripMenuItem.Click += new System.EventHandler(this.rightToolStripMenuItem_Click);
            // 
            // bottomToolStripMenuItem
            // 
            this.bottomToolStripMenuItem.Name = "bottomToolStripMenuItem";
            this.bottomToolStripMenuItem.Size = new System.Drawing.Size(166, 22);
            this.bottomToolStripMenuItem.Text = "Bottom";
            this.bottomToolStripMenuItem.Click += new System.EventHandler(this.bottomToolStripMenuItem_Click);
            // 
            // toolbarToolStripMenuItem
            // 
            this.toolbarToolStripMenuItem.Checked = true;
            this.toolbarToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.toolbarToolStripMenuItem.Name = "toolbarToolStripMenuItem";
            this.toolbarToolStripMenuItem.Size = new System.Drawing.Size(176, 22);
            this.toolbarToolStripMenuItem.Text = "Toolbar";
            this.toolbarToolStripMenuItem.Click += new System.EventHandler(this.toolbarToolStripMenuItem_Click);
            // 
            // statusbarToolStripMenuItem
            // 
            this.statusbarToolStripMenuItem.Checked = true;
            this.statusbarToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.statusbarToolStripMenuItem.Name = "statusbarToolStripMenuItem";
            this.statusbarToolStripMenuItem.Size = new System.Drawing.Size(176, 22);
            this.statusbarToolStripMenuItem.Text = "Statusbar";
            this.statusbarToolStripMenuItem.Click += new System.EventHandler(this.statusbarToolStripMenuItem_Click);
            // 
            // toolStripSeparator10
            // 
            this.toolStripSeparator10.Name = "toolStripSeparator10";
            this.toolStripSeparator10.Size = new System.Drawing.Size(173, 6);
            // 
            // wordWrapToolStripMenuItem
            // 
            this.wordWrapToolStripMenuItem.CheckOnClick = true;
            this.wordWrapToolStripMenuItem.Name = "wordWrapToolStripMenuItem";
            this.wordWrapToolStripMenuItem.Size = new System.Drawing.Size(176, 22);
            this.wordWrapToolStripMenuItem.Text = "Word Wrap";
            this.wordWrapToolStripMenuItem.Click += new System.EventHandler(this.wordWrapToolStripMenuItem_Click);
            // 
            // toolStripSeparator11
            // 
            this.toolStripSeparator11.Name = "toolStripSeparator11";
            this.toolStripSeparator11.Size = new System.Drawing.Size(173, 6);
            // 
            // increaseFontSizeToolStripMenuItem
            // 
            this.increaseFontSizeToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("increaseFontSizeToolStripMenuItem.Image")));
            this.increaseFontSizeToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.increaseFontSizeToolStripMenuItem.Name = "increaseFontSizeToolStripMenuItem";
            this.increaseFontSizeToolStripMenuItem.Size = new System.Drawing.Size(176, 22);
            this.increaseFontSizeToolStripMenuItem.Text = "Increase Font Size";
            this.increaseFontSizeToolStripMenuItem.Click += new System.EventHandler(this.increaseFontSizeToolStripMenuItem_Click);
            // 
            // decreaseFontSizeToolStripMenuItem
            // 
            this.decreaseFontSizeToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("decreaseFontSizeToolStripMenuItem.Image")));
            this.decreaseFontSizeToolStripMenuItem.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.decreaseFontSizeToolStripMenuItem.Name = "decreaseFontSizeToolStripMenuItem";
            this.decreaseFontSizeToolStripMenuItem.Size = new System.Drawing.Size(176, 22);
            this.decreaseFontSizeToolStripMenuItem.Text = "Decrease Font Size";
            this.decreaseFontSizeToolStripMenuItem.Click += new System.EventHandler(this.decreaseFontSizeToolStripMenuItem_Click);
            // 
            // resetFontSizeToolStripMenuItem
            // 
            this.resetFontSizeToolStripMenuItem.Name = "resetFontSizeToolStripMenuItem";
            this.resetFontSizeToolStripMenuItem.Size = new System.Drawing.Size(176, 22);
            this.resetFontSizeToolStripMenuItem.Text = "Reset Font Size";
            this.resetFontSizeToolStripMenuItem.Click += new System.EventHandler(this.resetFontSizeToolStripMenuItem_Click);
            // 
            // toolStripSeparator12
            // 
            this.toolStripSeparator12.Name = "toolStripSeparator12";
            this.toolStripSeparator12.Size = new System.Drawing.Size(173, 6);
            // 
            // whiteSpaceToolStripMenuItem
            // 
            this.whiteSpaceToolStripMenuItem.CheckOnClick = true;
            this.whiteSpaceToolStripMenuItem.Name = "whiteSpaceToolStripMenuItem";
            this.whiteSpaceToolStripMenuItem.Size = new System.Drawing.Size(176, 22);
            this.whiteSpaceToolStripMenuItem.Text = "White Space";
            this.whiteSpaceToolStripMenuItem.Click += new System.EventHandler(this.whiteSpaceToolStripMenuItem_Click);
            // 
            // lineBreaksToolStripMenuItem
            // 
            this.lineBreaksToolStripMenuItem.CheckOnClick = true;
            this.lineBreaksToolStripMenuItem.Name = "lineBreaksToolStripMenuItem";
            this.lineBreaksToolStripMenuItem.Size = new System.Drawing.Size(176, 22);
            this.lineBreaksToolStripMenuItem.Text = "Line Breaks";
            this.lineBreaksToolStripMenuItem.Click += new System.EventHandler(this.lineBreaksToolStripMenuItem_Click);
            // 
            // squirrelToolStripMenuItem
            // 
            this.squirrelToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.verifyToolStripMenuItem,
            this.executeToolStripMenuItem,
            this.terminateToolStripMenuItem,
            this.benchmarkToolStripMenuItem});
            this.squirrelToolStripMenuItem.Name = "squirrelToolStripMenuItem";
            this.squirrelToolStripMenuItem.Size = new System.Drawing.Size(59, 20);
            this.squirrelToolStripMenuItem.Text = "Squirrel";
            // 
            // verifyToolStripMenuItem
            // 
            this.verifyToolStripMenuItem.Enabled = false;
            this.verifyToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("verifyToolStripMenuItem.Image")));
            this.verifyToolStripMenuItem.Name = "verifyToolStripMenuItem";
            this.verifyToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F4;
            this.verifyToolStripMenuItem.Size = new System.Drawing.Size(193, 22);
            this.verifyToolStripMenuItem.Text = "Syntax Error Details";
            // 
            // executeToolStripMenuItem
            // 
            this.executeToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("executeToolStripMenuItem.Image")));
            this.executeToolStripMenuItem.Name = "executeToolStripMenuItem";
            this.executeToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F5;
            this.executeToolStripMenuItem.Size = new System.Drawing.Size(193, 22);
            this.executeToolStripMenuItem.Text = "Execute";
            this.executeToolStripMenuItem.Click += new System.EventHandler(this.executeToolStripMenuItem_Click);
            // 
            // terminateToolStripMenuItem
            // 
            this.terminateToolStripMenuItem.Enabled = false;
            this.terminateToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("terminateToolStripMenuItem.Image")));
            this.terminateToolStripMenuItem.Name = "terminateToolStripMenuItem";
            this.terminateToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F6;
            this.terminateToolStripMenuItem.Size = new System.Drawing.Size(193, 22);
            this.terminateToolStripMenuItem.Text = "Terminate";
            this.terminateToolStripMenuItem.Click += new System.EventHandler(this.terminateToolStripMenuItem_Click);
            // 
            // benchmarkToolStripMenuItem
            // 
            this.benchmarkToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("benchmarkToolStripMenuItem.Image")));
            this.benchmarkToolStripMenuItem.Name = "benchmarkToolStripMenuItem";
            this.benchmarkToolStripMenuItem.Size = new System.Drawing.Size(193, 22);
            this.benchmarkToolStripMenuItem.Text = "Benchmark";
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(189, 22);
            this.aboutToolStripMenuItem.Text = "About GeoGen Studio";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // toolStrip
            // 
            this.toolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.toolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripButton,
            this.openToolStripButton,
            this.saveToolStripButton,
            this.printToolStripButton,
            this.toolStripSeparator,
            this.cutToolStripButton,
            this.copyToolStripButton,
            this.pasteToolStripButton,
            this.toolStripSeparator1,
            this.verifyToolStripButton,
            this.executeToolStripButton,
            this.terminateToolStripButton,
            this.benchmarkToolStripButton});
            this.toolStrip.Location = new System.Drawing.Point(0, 24);
            this.toolStrip.Name = "toolStrip";
            this.toolStrip.Size = new System.Drawing.Size(1139, 25);
            this.toolStrip.TabIndex = 1;
            this.toolStrip.Text = "toolStrip1";
            // 
            // newToolStripButton
            // 
            this.newToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.newToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("newToolStripButton.Image")));
            this.newToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.newToolStripButton.Name = "newToolStripButton";
            this.newToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.newToolStripButton.Text = "&New";
            this.newToolStripButton.Click += new System.EventHandler(this.newToolStripMenuItem_Click);
            // 
            // openToolStripButton
            // 
            this.openToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.openToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("openToolStripButton.Image")));
            this.openToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.openToolStripButton.Name = "openToolStripButton";
            this.openToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.openToolStripButton.Text = "&Open";
            this.openToolStripButton.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // saveToolStripButton
            // 
            this.saveToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.saveToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("saveToolStripButton.Image")));
            this.saveToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.saveToolStripButton.Name = "saveToolStripButton";
            this.saveToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.saveToolStripButton.Text = "&Save";
            this.saveToolStripButton.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // printToolStripButton
            // 
            this.printToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.printToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("printToolStripButton.Image")));
            this.printToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.printToolStripButton.Name = "printToolStripButton";
            this.printToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.printToolStripButton.Text = "&Print";
            // 
            // toolStripSeparator
            // 
            this.toolStripSeparator.Name = "toolStripSeparator";
            this.toolStripSeparator.Size = new System.Drawing.Size(6, 25);
            // 
            // cutToolStripButton
            // 
            this.cutToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.cutToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("cutToolStripButton.Image")));
            this.cutToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.cutToolStripButton.Name = "cutToolStripButton";
            this.cutToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.cutToolStripButton.Text = "C&ut";
            this.cutToolStripButton.Click += new System.EventHandler(this.cutToolStripMenuItem_Click);
            // 
            // copyToolStripButton
            // 
            this.copyToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.copyToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("copyToolStripButton.Image")));
            this.copyToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.copyToolStripButton.Name = "copyToolStripButton";
            this.copyToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.copyToolStripButton.Text = "&Copy";
            this.copyToolStripButton.Click += new System.EventHandler(this.copyToolStripMenuItem_Click);
            // 
            // pasteToolStripButton
            // 
            this.pasteToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.pasteToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("pasteToolStripButton.Image")));
            this.pasteToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.pasteToolStripButton.Name = "pasteToolStripButton";
            this.pasteToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.pasteToolStripButton.Text = "&Paste";
            this.pasteToolStripButton.Click += new System.EventHandler(this.pasteToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // verifyToolStripButton
            // 
            this.verifyToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.verifyToolStripButton.Enabled = false;
            this.verifyToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("verifyToolStripButton.Image")));
            this.verifyToolStripButton.ImageTransparentColor = System.Drawing.Color.Black;
            this.verifyToolStripButton.Name = "verifyToolStripButton";
            this.verifyToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.verifyToolStripButton.Text = "Syntax error information";
            this.verifyToolStripButton.Click += new System.EventHandler(this.verifyToolStripButton_Click);
            // 
            // executeToolStripButton
            // 
            this.executeToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.executeToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("executeToolStripButton.Image")));
            this.executeToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.executeToolStripButton.Name = "executeToolStripButton";
            this.executeToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.executeToolStripButton.Text = "Execute the script";
            this.executeToolStripButton.Click += new System.EventHandler(this.executeToolStripMenuItem_Click);
            // 
            // terminateToolStripButton
            // 
            this.terminateToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.terminateToolStripButton.Enabled = false;
            this.terminateToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("terminateToolStripButton.Image")));
            this.terminateToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.terminateToolStripButton.Name = "terminateToolStripButton";
            this.terminateToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.terminateToolStripButton.Text = "Tterminate the script being executed";
            this.terminateToolStripButton.Click += new System.EventHandler(this.terminateToolStripMenuItem_Click);
            // 
            // benchmarkToolStripButton
            // 
            this.benchmarkToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.benchmarkToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("benchmarkToolStripButton.Image")));
            this.benchmarkToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.benchmarkToolStripButton.Name = "benchmarkToolStripButton";
            this.benchmarkToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.benchmarkToolStripButton.Text = "Benchmark the script";
            this.benchmarkToolStripButton.Click += new System.EventHandler(this.benchmarkToolStripButton_Click);
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.status,
            this.coords});
            this.statusStrip.Location = new System.Drawing.Point(0, 616);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(1139, 22);
            this.statusStrip.TabIndex = 2;
            this.statusStrip.Text = "statusStrip1";
            // 
            // status
            // 
            this.status.AutoSize = false;
            this.status.Name = "status";
            this.status.Size = new System.Drawing.Size(140, 17);
            this.status.Text = "Ready";
            // 
            // coords
            // 
            this.coords.AutoSize = false;
            this.coords.Name = "coords";
            this.coords.Size = new System.Drawing.Size(130, 17);
            // 
            // splitContainer
            // 
            this.splitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer.Location = new System.Drawing.Point(0, 49);
            this.splitContainer.Name = "splitContainer";
            // 
            // splitContainer.Panel1
            // 
            this.splitContainer.Panel1.Controls.Add(this.tabs);
            this.splitContainer.Panel1.RightToLeft = System.Windows.Forms.RightToLeft.No;
            // 
            // splitContainer.Panel2
            // 
            this.splitContainer.Panel2.Controls.Add(this.sidebarSplitContainer);
            this.splitContainer.Panel2.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.splitContainer.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.splitContainer.Size = new System.Drawing.Size(1139, 567);
            this.splitContainer.SplitterDistance = 918;
            this.splitContainer.TabIndex = 3;
            // 
            // tabs
            // 
            this.tabs.Controls.Add(codeTab);
            this.tabs.Controls.Add(this.outputTab);
            this.tabs.Controls.Add(this.tabPage1);
            this.tabs.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabs.Location = new System.Drawing.Point(0, 0);
            this.tabs.Name = "tabs";
            this.tabs.SelectedIndex = 0;
            this.tabs.Size = new System.Drawing.Size(918, 567);
            this.tabs.TabIndex = 0;
            // 
            // outputTab
            // 
            this.outputTab.Controls.Add(this.outputContainer);
            this.outputTab.Controls.Add(this.outputToolStrip);
            this.outputTab.Location = new System.Drawing.Point(4, 22);
            this.outputTab.Name = "outputTab";
            this.outputTab.Padding = new System.Windows.Forms.Padding(3);
            this.outputTab.Size = new System.Drawing.Size(910, 541);
            this.outputTab.TabIndex = 1;
            this.outputTab.Text = "Output";
            this.outputTab.UseVisualStyleBackColor = true;
            // 
            // outputContainer
            // 
            this.outputContainer.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("outputContainer.BackgroundImage")));
            this.outputContainer.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.outputContainer.Controls.Add(this.output);
            this.outputContainer.Cursor = System.Windows.Forms.Cursors.Cross;
            this.outputContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.outputContainer.Location = new System.Drawing.Point(3, 28);
            this.outputContainer.Name = "outputContainer";
            this.outputContainer.Size = new System.Drawing.Size(904, 510);
            this.outputContainer.TabIndex = 1;
            // 
            // output
            // 
            this.output.BackColor = System.Drawing.Color.Black;
            this.output.Cursor = System.Windows.Forms.Cursors.Cross;
            this.output.Location = new System.Drawing.Point(318, 166);
            this.output.Name = "output";
            this.output.Size = new System.Drawing.Size(179, 159);
            this.output.TabIndex = 0;
            this.output.TabStop = false;
            this.output.MouseMove += new System.Windows.Forms.MouseEventHandler(this.output_MouseMove);
            this.output.MouseDown += new System.Windows.Forms.MouseEventHandler(this.output_MouseDown);
            this.output.MouseUp += new System.Windows.Forms.MouseEventHandler(this.output_MouseUp);
            // 
            // outputToolStrip
            // 
            this.outputToolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.outputToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveOutput,
            this.clearOutputs,
            this.toolStripSeparator2,
            this.toolStripLabel1,
            this.outputs,
            this.toolStripSeparator3,
            this.toolStripLabel2,
            this.overlays,
            this.toggleOverlay,
            this.refreshOverlays});
            this.outputToolStrip.Location = new System.Drawing.Point(3, 3);
            this.outputToolStrip.Name = "outputToolStrip";
            this.outputToolStrip.Size = new System.Drawing.Size(904, 25);
            this.outputToolStrip.TabIndex = 0;
            this.outputToolStrip.Text = "toolStrip1";
            // 
            // saveOutput
            // 
            this.saveOutput.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.saveOutput.Enabled = false;
            this.saveOutput.Image = ((System.Drawing.Image)(resources.GetObject("saveOutput.Image")));
            this.saveOutput.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.saveOutput.Name = "saveOutput";
            this.saveOutput.Size = new System.Drawing.Size(23, 22);
            this.saveOutput.Text = "Save currently displayed image";
            this.saveOutput.Click += new System.EventHandler(this.saveOutput_Click);
            // 
            // clearOutputs
            // 
            this.clearOutputs.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.clearOutputs.Enabled = false;
            this.clearOutputs.Image = ((System.Drawing.Image)(resources.GetObject("clearOutputs.Image")));
            this.clearOutputs.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.clearOutputs.Name = "clearOutputs";
            this.clearOutputs.Size = new System.Drawing.Size(23, 22);
            this.clearOutputs.Text = "Discard all output data";
            this.clearOutputs.Click += new System.EventHandler(this.clearOutputs_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(90, 22);
            this.toolStripLabel1.Text = "Available maps:";
            // 
            // outputs
            // 
            this.outputs.Enabled = false;
            this.outputs.Name = "outputs";
            this.outputs.Size = new System.Drawing.Size(121, 25);
            this.outputs.SelectedIndexChanged += new System.EventHandler(this.outputs_SelectedIndexChanged);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripLabel2
            // 
            this.toolStripLabel2.Name = "toolStripLabel2";
            this.toolStripLabel2.Size = new System.Drawing.Size(85, 22);
            this.toolStripLabel2.Text = "Overlay views: ";
            // 
            // overlays
            // 
            this.overlays.Enabled = false;
            this.overlays.Name = "overlays";
            this.overlays.Size = new System.Drawing.Size(121, 25);
            this.overlays.SelectedIndexChanged += new System.EventHandler(this.overlays_SelectedIndexChanged);
            // 
            // toggleOverlay
            // 
            this.toggleOverlay.CheckOnClick = true;
            this.toggleOverlay.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toggleOverlay.Enabled = false;
            this.toggleOverlay.Image = ((System.Drawing.Image)(resources.GetObject("toggleOverlay.Image")));
            this.toggleOverlay.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toggleOverlay.Name = "toggleOverlay";
            this.toggleOverlay.Size = new System.Drawing.Size(23, 22);
            this.toggleOverlay.Text = "Toggle overlay display";
            this.toggleOverlay.Click += new System.EventHandler(this.toggleOverlay_Click);
            // 
            // refreshOverlays
            // 
            this.refreshOverlays.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.refreshOverlays.Enabled = false;
            this.refreshOverlays.Image = ((System.Drawing.Image)(resources.GetObject("refreshOverlays.Image")));
            this.refreshOverlays.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.refreshOverlays.Name = "refreshOverlays";
            this.refreshOverlays.Size = new System.Drawing.Size(23, 22);
            this.refreshOverlays.Text = "toolStripButton1";
            this.refreshOverlays.ToolTipText = "Refresh the list of available overlays";
            this.refreshOverlays.Click += new System.EventHandler(this.refreshOverlays_Click);
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.consoleLarge);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Size = new System.Drawing.Size(910, 541);
            this.tabPage1.TabIndex = 2;
            this.tabPage1.Text = "Log";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // consoleLarge
            // 
            this.consoleLarge.Dock = System.Windows.Forms.DockStyle.Fill;
            this.consoleLarge.Location = new System.Drawing.Point(0, 0);
            this.consoleLarge.Multiline = true;
            this.consoleLarge.Name = "consoleLarge";
            this.consoleLarge.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.consoleLarge.Size = new System.Drawing.Size(910, 541);
            this.consoleLarge.TabIndex = 0;
            this.consoleLarge.WordWrap = false;
            // 
            // sidebarSplitContainer
            // 
            this.sidebarSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.sidebarSplitContainer.Location = new System.Drawing.Point(0, 0);
            this.sidebarSplitContainer.Name = "sidebarSplitContainer";
            this.sidebarSplitContainer.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // sidebarSplitContainer.Panel1
            // 
            this.sidebarSplitContainer.Panel1.Controls.Add(this.parameters);
            this.sidebarSplitContainer.Panel1.Padding = new System.Windows.Forms.Padding(3);
            this.sidebarSplitContainer.Panel1.RightToLeft = System.Windows.Forms.RightToLeft.No;
            // 
            // sidebarSplitContainer.Panel2
            // 
            this.sidebarSplitContainer.Panel2.Controls.Add(this.console);
            this.sidebarSplitContainer.Panel2.Padding = new System.Windows.Forms.Padding(3);
            this.sidebarSplitContainer.Panel2.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.sidebarSplitContainer.Size = new System.Drawing.Size(217, 567);
            this.sidebarSplitContainer.SplitterDistance = 186;
            this.sidebarSplitContainer.TabIndex = 0;
            // 
            // parameters
            // 
            // 
            // 
            // 
            this.parameters.DocCommentDescription.AutoEllipsis = true;
            this.parameters.DocCommentDescription.Cursor = System.Windows.Forms.Cursors.Default;
            this.parameters.DocCommentDescription.Location = new System.Drawing.Point(3, 18);
            this.parameters.DocCommentDescription.Name = "";
            this.parameters.DocCommentDescription.Size = new System.Drawing.Size(205, 37);
            this.parameters.DocCommentDescription.TabIndex = 1;
            this.parameters.DocCommentImage = null;
            // 
            // 
            // 
            this.parameters.DocCommentTitle.Cursor = System.Windows.Forms.Cursors.Default;
            this.parameters.DocCommentTitle.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
            this.parameters.DocCommentTitle.Location = new System.Drawing.Point(3, 3);
            this.parameters.DocCommentTitle.Name = "";
            this.parameters.DocCommentTitle.Size = new System.Drawing.Size(205, 15);
            this.parameters.DocCommentTitle.TabIndex = 0;
            this.parameters.DocCommentTitle.UseMnemonic = false;
            this.parameters.Dock = System.Windows.Forms.DockStyle.Fill;
            this.parameters.Location = new System.Drawing.Point(3, 3);
            this.parameters.Name = "parameters";
            this.parameters.PropertySort = System.Windows.Forms.PropertySort.NoSort;
            this.parameters.Size = new System.Drawing.Size(211, 180);
            this.parameters.TabIndex = 0;
            this.parameters.ToolbarVisible = false;
            // 
            // 
            // 
            this.parameters.ToolStrip.AccessibleName = "ToolBar";
            this.parameters.ToolStrip.AccessibleRole = System.Windows.Forms.AccessibleRole.ToolBar;
            this.parameters.ToolStrip.AllowMerge = false;
            this.parameters.ToolStrip.AutoSize = false;
            this.parameters.ToolStrip.CanOverflow = false;
            this.parameters.ToolStrip.Dock = System.Windows.Forms.DockStyle.None;
            this.parameters.ToolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.parameters.ToolStrip.Location = new System.Drawing.Point(0, 1);
            this.parameters.ToolStrip.Name = "";
            this.parameters.ToolStrip.Padding = new System.Windows.Forms.Padding(2, 0, 1, 0);
            this.parameters.ToolStrip.Size = new System.Drawing.Size(234, 25);
            this.parameters.ToolStrip.TabIndex = 1;
            this.parameters.ToolStrip.TabStop = true;
            this.parameters.ToolStrip.Text = "PropertyGridToolBar";
            this.parameters.ToolStrip.Visible = false;
            // 
            // console
            // 
            this.console.BackColor = System.Drawing.SystemColors.ControlLight;
            this.console.Dock = System.Windows.Forms.DockStyle.Fill;
            this.console.Location = new System.Drawing.Point(3, 3);
            this.console.Multiline = true;
            this.console.Name = "console";
            this.console.ReadOnly = true;
            this.console.Size = new System.Drawing.Size(211, 371);
            this.console.TabIndex = 0;
            this.console.WordWrap = false;
            this.console.TextChanged += new System.EventHandler(this.console_TextChanged);
            // 
            // saveFile
            // 
            this.saveFile.FileName = "script.nut";
            this.saveFile.Filter = "Squirrel scripts (*.nut)|*.nut|All files|*.*";
            // 
            // openFile
            // 
            this.openFile.Filter = "Squirrel scripts (*.nut)|*.nut|All files|*.*";
            // 
            // saveOutputDialog
            // 
            this.saveOutputDialog.DefaultExt = "png";
            this.saveOutputDialog.Filter = "PNG Images|*.png|All files|*.*";
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1139, 638);
            this.Controls.Add(this.splitContainer);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.toolStrip);
            this.Controls.Add(this.menuStrip);
            this.DoubleBuffered = true;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip;
            this.Name = "Main";
            this.Text = "GeoGen Studio";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.Load += new System.EventHandler(this.Main_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Main_FormClosing);
            codeTab.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.editor)).EndInit();
            this.menuStrip.ResumeLayout(false);
            this.menuStrip.PerformLayout();
            this.toolStrip.ResumeLayout(false);
            this.toolStrip.PerformLayout();
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            this.splitContainer.Panel1.ResumeLayout(false);
            this.splitContainer.Panel2.ResumeLayout(false);
            this.splitContainer.ResumeLayout(false);
            this.tabs.ResumeLayout(false);
            this.outputTab.ResumeLayout(false);
            this.outputTab.PerformLayout();
            this.outputContainer.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.output)).EndInit();
            this.outputToolStrip.ResumeLayout(false);
            this.outputToolStrip.PerformLayout();
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.sidebarSplitContainer.Panel1.ResumeLayout(false);
            this.sidebarSplitContainer.Panel2.ResumeLayout(false);
            this.sidebarSplitContainer.Panel2.PerformLayout();
            this.sidebarSplitContainer.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip;
        private System.Windows.Forms.ToolStrip toolStrip;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem squirrelToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.SplitContainer splitContainer;
        private System.Windows.Forms.TabControl tabs;
        private System.Windows.Forms.TabPage outputTab;
        private ScintillaNet.Scintilla editor;
        private System.Windows.Forms.ToolStripButton newToolStripButton;
        private System.Windows.Forms.ToolStripButton openToolStripButton;
        private System.Windows.Forms.ToolStripButton saveToolStripButton;
        private System.Windows.Forms.ToolStripButton printToolStripButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator;
        private System.Windows.Forms.ToolStripButton cutToolStripButton;
        private System.Windows.Forms.ToolStripButton copyToolStripButton;
        private System.Windows.Forms.ToolStripButton pasteToolStripButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton verifyToolStripButton;
        private System.Windows.Forms.ToolStripButton executeToolStripButton;
        private System.Windows.Forms.ToolStripButton terminateToolStripButton;
        private System.Windows.Forms.ToolStripButton benchmarkToolStripButton;
        private System.Windows.Forms.ToolStrip outputToolStrip;
        private System.Windows.Forms.ToolStripButton saveOutput;
        private System.Windows.Forms.ToolStripButton clearOutputs;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripLabel toolStripLabel2;
        private System.Windows.Forms.ToolStripButton refreshOverlays;
        private System.Windows.Forms.Panel outputContainer;
        private System.Windows.Forms.SplitContainer sidebarSplitContainer;
        private System.Windows.Forms.TextBox console;
        private System.Windows.Forms.SaveFileDialog saveFile;
        private System.Windows.Forms.OpenFileDialog openFile;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripMenuItem printToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem undoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem redoToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator6;
        private System.Windows.Forms.ToolStripMenuItem cutToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem copyToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem pasteToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator7;
        private System.Windows.Forms.ToolStripMenuItem searchAndReplaceToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator8;
        private System.Windows.Forms.ToolStripMenuItem settingsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem codeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem outputToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem documentationToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator9;
        private System.Windows.Forms.ToolStripMenuItem sidebarToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem scriptParametersToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem consoleToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolbarToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem statusbarToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator10;
        private System.Windows.Forms.ToolStripMenuItem wordWrapToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator11;
        private System.Windows.Forms.ToolStripMenuItem increaseFontSizeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem decreaseFontSizeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem resetFontSizeToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator12;
        private System.Windows.Forms.ToolStripMenuItem whiteSpaceToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem lineBreaksToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator13;
        private System.Windows.Forms.ToolStripMenuItem leftToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem rightToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem bottomToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem verifyToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem executeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem terminateToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem benchmarkToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        public System.Windows.Forms.ToolStripComboBox outputs;
        public System.Windows.Forms.PictureBox output;
        public System.Windows.Forms.ToolStripComboBox overlays;
        private System.Windows.Forms.ToolStripStatusLabel status;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TextBox consoleLarge;
        public System.Windows.Forms.ToolStripButton toggleOverlay;
        private System.Windows.Forms.ToolStripStatusLabel coords;
        public PropertyGridEx.PropertyGridEx parameters;
        public System.Windows.Forms.SaveFileDialog saveOutputDialog;
        private System.Windows.Forms.ToolStripMenuItem logToolStripMenuItem;
    }
}

