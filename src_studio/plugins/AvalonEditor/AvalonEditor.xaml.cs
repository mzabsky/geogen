namespace GeoGen.Studio.PlugIns
{
    using System;
    using System.ComponentModel;
    using System.IO;
    using System.Windows;
    using System.Windows.Data;
    using System.Windows.Input;
    using System.Xml;

    using GeoGen.Studio.PlugInLoader;
    using GeoGen.Studio.PlugIns.Extensions;
    using GeoGen.Studio.PlugIns.Interfaces;
    using GeoGen.Studio.PlugIns.MenuBars;
    using GeoGen.Studio.PlugIns.StatusBars;
    using GeoGen.Studio.PlugIns.ToolBars;
    using GeoGen.Studio.Utilities;
    using GeoGen.Studio.Utilities.Context;
    using GeoGen.Studio.Utilities.Extensions;
    using GeoGen.Studio.Utilities.IO;
    using GeoGen.Studio.Utilities.Messaging;
    using GeoGen.Studio.Utilities.Persistence;

    using ICSharpCode.AvalonEdit.Highlighting;
    using ICSharpCode.AvalonEdit.Highlighting.Xshd;

    // Resharper doesn't like the checks in properties which update the inner editor properties
    // ReSharper disable RedundantCheckBeforeAssignment

    /// <summary>
    /// Single-tab code editor.
    /// </summary>
    [PlugIn(
        Name = "Code Editor",
        Description = "Editor of map script source code. Supports syntax highlighting, word wrapping and other essential"
                      + " features.",
        VisibleInList = true)]
    public partial class AvalonEditor : GeoGen.Studio.Utilities.PlugInBase.ControlBase, ITextProvider, IEditor, INotifyPropertyChanged
    {
        /// <summary>
        /// Path where for this plug-in icons are found.
        /// </summary>
        public const string IconPathPrefix = "pack://application:,,,/GeoGen.Studio.PlugIns.AvalonEditor;component/Images/Icons/";

        /// <summary>
        /// <see cref="Context"/> used to display caret position to the status bar.
        /// </summary>
        private readonly Context editorContext = new Context();

        /*private bool enablePropertyCallbacks = true;*/

        /*private ICSharpCode.AvalonEdit.Folding.AbstractFoldingStrategy foldingStrategy;
        private ICSharpCode.AvalonEdit.Folding.FoldingManager foldingManager;
        private Guid currentFileSessionGuid;*/

        /// <summary>
        /// Dock manager in which is this plug-in displayed.
        /// </summary>
        private IDockManager dockManager;

        /// <summary>
        /// Backing field for <see cref="NewCommand"/>.
        /// </summary>
        private ICommand newCommand;

        /// <summary>
        /// Backing field for <see cref="OpenCommand"/>.
        /// </summary>
        private ICommand openCommand;

        /// <summary>
        /// Backing field for <see cref="SaveCommand"/>.
        /// </summary>
        private ICommand saveCommand;

        /// <summary>
        /// Backing field for <see cref="SaveAsCommand"/>.
        /// </summary>
        private ICommand saveAsCommand;

        /// <summary>
        /// Backing field for <see cref="IncreaseFontSizeCommand"/>.
        /// </summary>
        private ICommand increaseFontSizeCommand;

        /// <summary>
        /// Backing field for <see cref="DecreaseFontSizeCommand"/>.
        /// </summary>
        private ICommand decreaseFontSizeCommand;

        /// <summary>
        /// Backing field for <see cref="ResetFontSizeCommand"/>.
        /// </summary>
        private ICommand resetFontSizeCommand;

        /// <summary>
        /// Backing field for <see cref="ToggleWordWrapCommand"/>.
        /// </summary>
        private ICommand toggleWordWrapCommand;

        /// <summary>
        /// Backing field for <see cref="UndoCommand"/>.
        /// </summary>
        private ICommand undoCommand;

        /// <summary>
        /// Backing field for <see cref="RedoCommand"/>.
        /// </summary>
        private ICommand redoCommand;

        /// <summary>
        /// Backing field for <see cref="ShowEditorCommand"/>.
        /// </summary>
        private ICommand showEditorCommand;

        /// <summary>
        /// Backing field for <see cref="CaretLine"/>.
        /// </summary>
        private int caretLine;

        /// <summary>
        /// Backing field for <see cref="CaretColumn"/>.
        /// </summary>
        private int caretColumn;

        /// <summary>
        /// Backing field for <see cref="CaretOffset"/>.
        /// </summary>
        private int caretOffset;

        /// <summary>
        /// Backing field for <see cref="SelectionStart"/>.
        /// </summary>
        private int selectionStart;

        /// <summary>
        /// Backing field for <see cref="SelectionEnd"/>.
        /// </summary>
        private int selectionEnd;

        /// <summary>
        /// Backing field for <see cref="SelectionLength"/>.
        /// </summary>
        private int selectionLength;

        /// <summary>
        /// Backing field for <see cref="WordWrap"/>.
        /// </summary>
        private bool wordWrap;

        /// <summary>
        /// Initializes a new instance of the <see cref="AvalonEditor"/> class.
        /// </summary>
        public AvalonEditor()
        {                        
            InitializeComponent();
            MainConfig.Register(this);

            // load the template and mark the content as unsaved
            this.New();

            this.editor.TextArea.SnapsToDevicePixels = true;

            this.InitializeSyntaxHighlighing();

            // Code folding
            /*
            this.foldingManager = ICSharpCode.AvalonEdit.Folding.FoldingManager.Install(editor.TextArea);
            this.foldingStrategy = new BraceFoldingStrategy();
            this.foldingStrategy.UpdateFoldings(this.foldingManager, this.editor.Document);
            */            

            this.editor.TextChanged += this.HandleTextChanged;
            this.editor.TextArea.TextEntered += this.HandleTextChanged;
            this.editor.TextArea.SelectionChanged += this.HandleSelectionChanged;
            this.editor.TextArea.Caret.PositionChanged += this.HandleSelectionChanged;
            this.editor.Loaded += this.HandleEditorLoaded;

            this.editor.GotFocus += delegate
            {
                ContextManager.EnterContext(this.editorContext);
            };

            this.editor.LostFocus += delegate
            {
                ContextManager.LeaveContext(this.editorContext);
            };

            FileService.FileCreated += delegate(object o, FileEventArgs args)
            {
                if (args.FileSession.Extension == ".nut")
                {
                    this.editor.Text = File.ReadAllText(this.TemplateFile);
                    this.CurrentFileName = this.LastFileName = args.FileSession.FileInfo.FullName;
                    ////this.currentFileSessionGuid = args.FileSession.FileSessionGuid;

                    this.IsUnsaved = false;

                    // switch to this tab after creating a file
                    this.Activate();
                }
            };

            FileService.FileOpened += delegate(object o, FileEventArgs args)
            {
                if (args.FileSession.FileInfo.IsTextFile() || args.FileSession.Extension == ".nut")
                {               
                    this.editor.Text = File.ReadAllText(args.FileSession.FileInfo.FullName);
                    this.CurrentFileName = this.LastFileName = args.FileSession.FileInfo.FullName;
                    ////this.currentFileSessionGuid = args.FileSession.FileSessionGuid;

                    this.IsUnsaved = false;

                    // switch to this tab after opening a file
                    this.Activate();
                }
            };
        }

        /// <summary>
        /// Gets the <see cref="New"/> command.
        /// </summary>
        public ICommand NewCommand
        {
            get
            {
                if (this.newCommand == null)
                {
                    this.newCommand = new RelayCommand(param => this.New());
                }

                return this.newCommand;
            }
        }

        /// <summary>
        /// Gets the <see cref="Open"/> command.
        /// </summary>
        public ICommand OpenCommand
        {
            get
            {
                if (this.openCommand == null)
                {
                    this.openCommand = new RelayCommand(param => this.Open());
                }

                return this.openCommand;
            }
        }

        /// <summary>
        /// Gets the <see cref="Save"/> command.
        /// </summary>
        public ICommand SaveCommand
        {
            get
            {
                if (this.saveCommand == null)
                {
                    this.saveCommand = new RelayCommand(param => this.Save());
                }

                return this.saveCommand;
            }
        }

        /// <summary>
        /// Gets the <see cref="SaveAs"/> command.
        /// </summary>
        public ICommand SaveAsCommand
        {
            get
            {
                if (this.saveAsCommand == null)
                {
                    this.saveAsCommand = new RelayCommand(param => this.SaveAs());
                }

                return this.saveAsCommand;
            }
        }

        /// <summary>
        /// Gets command which increases font size.
        /// </summary>
        public ICommand IncreaseFontSizeCommand
        {
            get
            {
                if (this.increaseFontSizeCommand == null)
                {
                    this.increaseFontSizeCommand = new RelayCommand(param => ++this.FontSize);
                }

                return this.increaseFontSizeCommand;
            }
        }

        /// <summary>
        /// Gets command which decreases font size.
        /// </summary>
        public ICommand DecreaseFontSizeCommand
        {
            get
            {
                if (this.decreaseFontSizeCommand == null)
                {
                    this.decreaseFontSizeCommand = new RelayCommand(param => --this.FontSize);
                }

                return this.decreaseFontSizeCommand;
            }
        }

        /// <summary>
        /// Gets command which resets font size to <see cref="DefaultFontSize"/>.
        /// </summary>
        public ICommand ResetFontSizeCommand
        {
            get
            {
                if (this.resetFontSizeCommand == null)
                {
                    this.resetFontSizeCommand = new RelayCommand(param => this.FontSize = this.DefaultFontSize);
                }

                return this.resetFontSizeCommand;
            }
        }

        /// <summary>
        /// Gets the command which toggles <see cref="WordWrap"/>.
        /// </summary>
        public ICommand ToggleWordWrapCommand
        {
            get
            {
                if (this.toggleWordWrapCommand == null)
                {
                    this.toggleWordWrapCommand = new RelayCommand(param => this.WordWrap = !this.WordWrap);
                }

                return this.toggleWordWrapCommand;
            }
        }

        /// <summary>
        /// Gets the undo command.
        /// </summary>
        public ICommand UndoCommand
        {
            get
            {
                if (this.undoCommand == null)
                {
                    this.undoCommand = new RelayCommand(param => this.editor.Undo(), param => this.editor.CanUndo);
                }

                return this.undoCommand;
            }
        }

        /// <summary>
        /// Gets the redo command.
        /// </summary>
        public ICommand RedoCommand
        {
            get
            {
                if (this.redoCommand == null)
                {
                    this.redoCommand = new RelayCommand(param => this.editor.Redo(), param => this.editor.CanRedo);
                }

                return this.redoCommand;
            }
        }

        /// <summary>
        /// Gets the <see cref="Activate">show editor</see> command.
        /// </summary>
        public ICommand ShowEditorCommand
        {
            get
            {
                if (this.showEditorCommand == null)
                {
                    this.showEditorCommand = new RelayCommand(param => this.Activate());
                }

                return this.showEditorCommand;
            }
        }

        /// <summary>
        /// Gets or sets the text contained in the editor.
        /// </summary>
        /// <value>
        /// The text.
        /// </value>
        public string Text
        {
            get
            {
                return this.editor.Text;
            }

            set
            {
                this.editor.Text = value;
            }
        }

        /*private static void OnTextChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var avalonEditor = d as AvalonEditor;

            if (avalonEditor != null)
            {
                if (avalonEditor.enablePropertyCallbacks) avalonEditor.editor.Text = avalonEditor.Text;
            }
        }*/

        /// <summary>
        /// Gets the name of the currently opened file in the editor. Empty string if no file is opened in the editor (
        /// for example because it was not saved yet).
        /// </summary>
        /// <value>
        /// The name of the current file.
        /// </value>
        public string CurrentFileName { get; private set; }

        /// <summary>
        /// Gets or sets name of the last file opened or saved with the editor.
        /// </summary>
        /// <value>
        /// The last name of the file.
        /// </value>
        [Persistent("")]
        public string LastFileName { get; set; }

        /// <summary>
        /// Gets or sets name of the file used as a template.
        /// </summary>
        /// <value>
        /// The template file.
        /// </value>
        [Persistent("../examples/template.nut")]
        public string TemplateFile { get; set; }

        /// <summary>
        /// Gets a value indicating whether there are unsaved changes in the file.
        /// </summary>
        /// <value>
        ///     <c>true</c> if there are unsaved file; otherwise, <c>false</c>.
        /// </value>
        public bool IsUnsaved { get; private set; }

        /// <summary>
        /// Gets or sets the number of the line on which is the caret currently located.
        /// </summary>
        /// <value>
        /// The caret line.
        /// </value>
        public int CaretLine
        {
            get
            {
                return this.caretLine;
            }

            set
            {
                this.caretLine = value;

                if (this.editor.TextArea.Caret.Line != value)
                {
                    this.editor.TextArea.Caret.Line = value;
                }
            }
        }

        /// <summary>
        /// Gets or sets the number of the column on which is the caret currently located.
        /// </summary>
        /// <value>
        /// The caret column.
        /// </value>
        public int CaretColumn
        {
            get
            {
                return this.caretColumn;
            }

            set
            {
                this.caretColumn = value;

                if (this.editor.TextArea.Caret.Column != value)
                {
                    this.editor.TextArea.Caret.Column = value;
                }
            }
        }

        /// <summary>
        /// Gets or sets the position of the caret within <see cref="Text"/>.
        /// </summary>
        /// <value>
        /// The caret offset.
        /// </value>
        public int CaretOffset
        {
            get
            {
                return this.caretOffset;
            }

            set
            {
                this.caretOffset = value;

                if (this.editor.TextArea.Caret.Offset != value)
                {
                    this.editor.TextArea.Caret.Offset = value;
                }
            }
        }

        /// <summary>
        /// Gets or sets the position of first character in selection within <see cref="Text"/>.
        /// </summary>
        /// <value>
        /// The selection start.
        /// </value>
        public int SelectionStart
        {
            get
            {
                return this.selectionStart;
            }

            set
            {
                this.selectionStart = value;

                if (this.editor.SelectionStart != value)
                {
                    this.editor.SelectionStart = value;    
                }
            }
        }

        /// <summary>
        /// Gets or sets the position of last character in selection within <see cref="Text"/>.
        /// </summary>
        /// <value>
        /// The selection start.
        /// </value>
        public int SelectionEnd
        {
            get
            {
                return this.selectionEnd;
            }

            set
            {
                this.selectionEnd = value;

                if (this.editor.SelectionStart != value - this.editor.SelectionLength)
                {
                    this.editor.SelectionStart = value - this.editor.SelectionLength;
                }                
            }
        }

        /// <summary>
        /// Gets or sets the length of the selection.
        /// </summary>
        /// <value>
        /// The length of the selection.
        /// </value>
        public int SelectionLength
        {
            get
            {
                return this.selectionLength;
            }

            set
            {
                this.selectionLength = value;

                if (this.editor.SelectionLength != value)
                {
                    this.editor.SelectionLength = value;    
                }
            }
        }

        /// <summary>
        /// Gets or sets the default size of the font (to which the font size returns after <see cref="ResetFontSizeCommand"/>).
        /// </summary>
        /// <value>
        /// The default size of the font.
        /// </value>
        [Persistent(13)]
        public int DefaultFontSize { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether word wrap is enabled.
        /// </summary>
        /// <value>
        ///   <c>true</c> if [word wrap]; otherwise, <c>false</c>.
        /// </value>
        [Persistent(false)]
        public bool WordWrap
        {
            get
            {
                return this.wordWrap;
            }

            set
            {
                this.wordWrap = value;

                if (this.editor.WordWrap != value)
                {
                    this.editor.WordWrap = value;
                }
            }
        }

        /// <summary>
        /// Add the editor as tab to the main interface.
        /// </summary>
        /// <param name="dockManager">The dock manager.</param>
        public void Register(IDockManager dockManager)
        {
            this.dockManager = dockManager;
            dockManager.AddAsDocumentContent(this, "Code", true);
        }

        /// <summary>
        /// Register global hotkeys.
        /// </summary>
        /// <param name="mainWindow">The main window.</param>
        public void Register(IMainWindow mainWindow)
        {
            mainWindow.RegisterInputGesture(
                new KeyGesture(Key.N, ModifierKeys.Control),
                this.NewCommand);

            mainWindow.RegisterInputGesture(
                new KeyGesture(Key.O, ModifierKeys.Control),
                this.OpenCommand);

            mainWindow.RegisterInputGesture(
                new KeyGesture(Key.S, ModifierKeys.Control),
                this.SaveCommand);

            mainWindow.RegisterInputGesture(
                new KeyGesture(Key.S, ModifierKeys.Control | ModifierKeys.Shift),
                this.SaveAsCommand);
        }

        /// <summary>
        /// Registers the editor into main window menu.
        /// </summary>
        /// <param name="menuBar">The menu bar.</param>
        public void Register(IMenuBar menuBar)
        {            
            menuBar.AddMenu(
                new MenuEntry(
                    header: "File",
                    items: new MenuEntryObservableCollection
                    {
                        new MenuEntry(
                            header: "New",
                            priority: 0,
                            command: this.NewCommand,
                            inputGestureText: "Ctrl+N",
                            icon: AvalonEditor.IconPathPrefix + "new.png"),
                        new MenuEntry(
                            header: "Open",
                            priority: -1,
                            command: this.OpenCommand,
                            inputGestureText: "Ctrl+O",
                            icon: AvalonEditor.IconPathPrefix + "open.png"),
                        new MenuEntry(
                            header: "Save",
                            priority: -2,
                            command: this.SaveCommand,
                            inputGestureText: "Ctrl+S",
                            icon: AvalonEditor.IconPathPrefix + "save.png"),
                        new MenuEntry(
                            header: "Save As...",
                            priority: -3,
                            command: this.SaveAsCommand,
                            inputGestureText: "Ctrl+Shift+S",
                            icon: AvalonEditor.IconPathPrefix + "save.png"),
                        new MenuSeparator(
                            priority: -4)
                    }));

            menuBar.AddMenu(
                new MenuEntry(
                    header: "Edit",
                    priority: -1,
                    items: new MenuEntryObservableCollection
                        {
                        new MenuEntry(
                            header: "Undo",
                            priority: 0,
                            command: this.UndoCommand,
                            inputGestureText: "Ctrl+Z",
                            icon: AvalonEditor.IconPathPrefix + "undo.png"),
                        new MenuEntry(
                            header: "Redo",
                            priority: -1,
                            command: this.RedoCommand,
                            inputGestureText: "Ctrl+Shift+Z",
                            icon: AvalonEditor.IconPathPrefix + "redo.png"),
                        new MenuSeparator(
                            priority: -2),
                        new MenuEntry(
                            header: "Cut",
                            priority: -3,
                            command: ApplicationCommands.Cut,
                            inputGestureText: "Ctrl+X",
                            icon: AvalonEditor.IconPathPrefix + "cut.png"),
                        new MenuEntry(
                            header: "Copy",
                            priority: -4,
                            command: ApplicationCommands.Copy,
                            inputGestureText: "Ctrl+C",
                            icon: AvalonEditor.IconPathPrefix + "copy.png"),
                        new MenuEntry(
                            header: "Paste",
                            priority: -5,
                            command: ApplicationCommands.Paste,
                            inputGestureText: "Ctrl+P",
                            icon: AvalonEditor.IconPathPrefix + "paste.png")
                    }));

            menuBar.AddMenu(
                new MenuEntry(
                    header: "View",
                    priority: -2,
                    items: new MenuEntryObservableCollection
                    {
                        new MenuEntry(
                            header: "Code",
                            priority: 0,
                            command: this.ShowEditorCommand,
                            icon: AvalonEditor.IconPathPrefix + "code.png"),
                        new MenuSeparator(
                            priority: -20),
                        new MenuEntry(
                            header: "Word Wrap",
                            priority: -21,
                            command: this.ToggleWordWrapCommand,
                            isCheckable: true,
                            dataContext: this,
                            isCheckedBinding: new Binding("WordWrap")),
                        new MenuSeparator(
                            priority: -22),
                        new MenuEntry(
                            header: "Increase Font Size",
                            priority: -23,
                            command: this.IncreaseFontSizeCommand),
                        new MenuEntry(
                            header: "Decrease Font Size",
                            priority: -24,
                            command: this.DecreaseFontSizeCommand),
                        new MenuEntry(
                            header: "Reset Font Size",
                            priority: -25,
                            command: this.ResetFontSizeCommand),
                    }));
        }

        /// <summary>
        /// Adds buttons to the main window toolbar.
        /// </summary>
        /// <param name="toolBar">The tool bar.</param>
        public void Register(IMainWindowToolBar toolBar)
        {
            toolBar.AddItem(new ToolBarButton(
                icon: AvalonEditor.IconPathPrefix + "new.png",
                priority: 0,
                toolTip: "New Script (Ctrl+N)",
                command: this.NewCommand));

            toolBar.AddItem(new ToolBarButton(
                icon: AvalonEditor.IconPathPrefix + "open.png",
                priority: -1,
                toolTip: "Open Script (Ctrl+O)",
                command: this.OpenCommand));

            toolBar.AddItem(new ToolBarButton(
                icon: AvalonEditor.IconPathPrefix + "save.png",
                priority: -2,
                toolTip: "Save Script (Ctrl+S)",
                command: this.SaveCommand));

            toolBar.AddItem(new ToolBarSeparator(
                priority: -3));

            toolBar.AddItem(new ToolBarButton(
                icon: AvalonEditor.IconPathPrefix + "cut.png",
                priority: -4,
                toolTip: "Cut (Ctrl+X)",
                command: ApplicationCommands.Cut));

            toolBar.AddItem(new ToolBarButton(
                icon: AvalonEditor.IconPathPrefix + "copy.png",
                priority: -5,
                toolTip: "Copy (Ctrl+C)",
                command: ApplicationCommands.Copy));

            toolBar.AddItem(new ToolBarButton(
                icon: AvalonEditor.IconPathPrefix + "paste.png",
                priority: -6,
                toolTip: "Paste (Ctrl+P)",
                command: ApplicationCommands.Paste));

            toolBar.AddItem(new ToolBarSeparator(
                priority: -7));
        }

        /// <summary>
        /// Add caret position to the status bar.
        /// </summary>
        /// <param name="statusBar">The status bar.</param>
        public void Register(IStatusBar statusBar)
        {
            var valueBinding = new MultiBinding();
            valueBinding.StringFormat = "{0} x {1} [{2}]";
            valueBinding.Bindings.Add(
                new Binding
                {
                    Path = new PropertyPath("CaretLine"),
                    Source = this
                });
            valueBinding.Bindings.Add(
                new Binding
                {
                    Path = new PropertyPath("CaretColumn"),
                    Source = this
                });
            valueBinding.Bindings.Add(
                new Binding
                {
                    Path = new PropertyPath("SelectionLength"),
                    Source = this
                });

            statusBar.AddItem(
                new StatusBarEntry
                {
                    Context = this.editorContext,
                    ValueBinding = valueBinding
                });
        }

        /// <summary>
        /// Defines quick actions (for example to th welcome screen).
        /// </summary>
        /// <param name="quickActionDisplay">The quick action display.</param>
        public void Register(IQuickActionDisplay quickActionDisplay)
        {
            quickActionDisplay.RegisterQuickAction(new QuickAction
            {
                Label = "Create new map script",
                Command = new RelayCommand(p => this.New())
            });

            quickActionDisplay.RegisterQuickAction(new QuickAction
            {
                Label = "Open map script",
                Command = new RelayCommand(p => this.Open())
            });
        }

        /// <summary>
        /// Creates a new file (without saving it). Uses a default templae
        /// </summary>
        public void New()
        {
            if (this.TemplateFile == string.Empty)
            {
                this.editor.Text = string.Empty;
            }
            else
            {
                try
                {
                    this.editor.Text = System.IO.File.ReadAllText(this.TemplateFile);
                }
                catch (IOException)
                {
                    new Message("Could not open template file.", MessageType.Warning).Send();
                    this.editor.Text = string.Empty;
                }
            }

            this.CurrentFileName = string.Empty;
            this.IsUnsaved = false;

            // switch to this tab after creating a new file
            if (this.dockManager != null)
            {
                this.dockManager.Activate(this);
            }
        }

        /// <summary>
        /// Saves the currently opened file (offers to choose the file to save if it was not chosen yet).
        /// </summary>
        public void Save()
        {
            try
            {
                if (this.CurrentFileName == string.Empty)
                {
                    this.CurrentFileName =
                        this.LastFileName =
                        FileDialog.ShowSave(this.LastFileName, "Squirrel Scripts (*.nut)|*.nut|All files|*.*");
                }

                File.WriteAllText(this.CurrentFileName, this.editor.Text);

                this.IsUnsaved = false;
            }
            catch (IOException)
            {
                new Message("Failed to save " + this.LastFileName + ".", MessageType.Error).Send();
            }
        }

        /// <summary>
        /// Saves the currently opened file, will ask the user to choose the file.
        /// </summary>
        public void SaveAs()
        {           
            try
            {
                this.CurrentFileName = this.LastFileName = FileDialog.ShowSave(this.LastFileName, "Squirrel Scripts (*.nut)|*.nut|All files|*.*");
                
                File.WriteAllText(this.CurrentFileName, this.editor.Text);

                this.IsUnsaved = false;                
            }
            catch (IOException)
            {
                new Message("Failed to save " + this.LastFileName + ".", MessageType.Error).Send();
            }          
        }

        /// <summary>
        /// Asks the user to choose a new file.
        /// </summary>
        public void Open()
        {
            try
            {
                string fileName = FileDialog.ShowOpen(this.LastFileName, "Squirrel Scripts (*.nut)|*.nut|All files|*.*");                

                FileService.OnFileOpened(this, new FileInfo(fileName));             
            }
            catch (IOException)
            {
                new Message("File \"" + this.LastFileName + "\" is not readable", MessageType.Error).Send();
            }
        }

        /// <summary>
        /// Replaces part of the test with another string.
        /// </summary>
        /// <param name="startOffset">The start offset.</param>
        /// <param name="length">The length.</param>
        /// <param name="replace">The replace.</param>
        public void ReplaceString(int startOffset, int length, string replace)
        {
            this.editor.Document.Replace(startOffset, length, replace);
        }

        /// <summary>
        /// Brings the editor sub-window to focus.
        /// </summary>
        public void Activate()
        {
            if (this.dockManager != null)
            {
                this.dockManager.Activate(this);
            }
        }

        /// <summary>
        /// Initializes the syntax highlighter.
        /// </summary>
        private void InitializeSyntaxHighlighing()
        {
            try
            {
                using (var reader = new XmlTextReader("squirrel.xshd"))
                {
                    this.editor.SyntaxHighlighting = HighlightingLoader.Load(reader, HighlightingManager.Instance);                
                }
            }
            catch (FileNotFoundException)
            {
                new Message("Could not read syntax highlighter definitions file.", MessageType.Warning).Send();
            }
        }

        /// <summary>
        /// Registers window closing handler (to make sure the editor asks the user to save unsaved work).
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="e">The <see cref="System.EventArgs"/> instance containing the event data.</param>
        private void HandleEditorLoaded(object sender, EventArgs e)
        {
            // The window may be null (for example in design mode).
            var window = Window.GetWindow(this);
            if (window != null)
            {
                // we want to ask the user if he wants to save unsaved content
                // but prevent repeated even registration (loaded can be called repeatedly)
                window.Closing -= this.HandleWindowClosing;
                window.Closing += this.HandleWindowClosing;
            }
        }

        /// <summary>
        /// Asks the user to save his work, if necessary.
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="e">The <see cref="System.ComponentModel.CancelEventArgs"/> instance containing the event data.</param>
        private void HandleWindowClosing(object sender, CancelEventArgs e)
        {
            if (this.IsUnsaved)
            {
                var result = UI.MessageBox.Show("Do you wish to save unsaved content?", "GeoGen Studio", MessageBoxButton.YesNoCancel, MessageBoxImage.Information);                

                switch (result)
                {
                    case MessageBoxResult.Yes:
                        try
                        {
                            this.Save();
                        }
                        catch (Exception)
                        {
                            e.Cancel = true;
                        }

                        break;
                    case MessageBoxResult.No:
                        break;
                    case MessageBoxResult.Cancel:
                        e.Cancel = true;
                        break;
                }
            }            
        }

        /// <summary>
        /// Marks the text as unsaved.
        /// </summary>
        /// <param name="sender">
        /// The sender.
        /// </param>
        /// <param name="e">
        /// The args.
        /// </param>
        private void HandleTextChanged(object sender, EventArgs e)
        {
            this.IsUnsaved = true;
            this.OnPropertyChanged("Text");
            ////this.foldingStrategy.UpdateFoldings(this.foldingManager, this.editor.Document);
        }

        /// <summary>
        /// Synchronizes outer text cursor to the innter AvalonEdit component.
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="e">The <see cref="System.EventArgs"/> instance containing the event data.</param>
        private void HandleSelectionChanged(object sender, EventArgs e)
        {
            this.SelectionStart = this.editor.SelectionStart;
            this.SelectionEnd = this.editor.SelectionStart + this.editor.SelectionLength;
            this.SelectionLength = this.editor.SelectionLength;
            this.CaretColumn = this.editor.TextArea.Caret.Column;
            this.CaretLine = this.editor.TextArea.Caret.Line;
        }
    }
}