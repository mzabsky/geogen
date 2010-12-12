using System;
using System.Xml;
using System.Runtime.Serialization;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
using ICSharpCode.AvalonEdit.Editing;
using GeoGen.Studio.PlugInLoader;
using GeoGen.Studio.Utilities;
using GeoGen.Studio.Utilities.Messaging;
using GeoGen.Studio.Utilities.Configurability;

namespace GeoGen.Studio.PlugIns
{
    public partial class AvalonEditor : GeoGen.Studio.Utilities.PlugInBase.Control, IConfigurable, ITextProvider, IEditor
    {
        #region Dependency properties
        public static readonly DependencyProperty TextProperty = DependencyProperty.Register(
            "Text", typeof(string), typeof(AvalonEditor), new FrameworkPropertyMetadata("", new PropertyChangedCallback(OnTextChanged))
        );

        public string Text
        {
            get
            {
                return (string)GetValue(TextProperty);
            }
            set
            {
                SetValue(TextProperty, value);
            }
        }

        private static void OnTextChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            if (((AvalonEditor)d).enablePropertyCallbacks) ((AvalonEditor)d).editor.Text = ((AvalonEditor)d).Text;
        }

        public static readonly DependencyPropertyKey CurrentFileNamePropertyKey = DependencyProperty.RegisterReadOnly(
            "CurrentFileName", typeof(string), typeof(AvalonEditor), new PropertyMetadata(""));

        public static readonly DependencyProperty CurrentFileNameProperty = CurrentFileNamePropertyKey.DependencyProperty;

        public string CurrentFileName
        {
            get
            {
                return (string)GetValue(CurrentFileNameProperty);
            }
            private set
            {
                SetValue(CurrentFileNamePropertyKey, value);
            }
        }

        public static readonly DependencyPropertyKey LastFileNamePropertyKey = DependencyProperty.RegisterReadOnly(
            "LastFileName", typeof(string), typeof(AvalonEditor), new PropertyMetadata(""));

        public static readonly DependencyProperty LastFileNameProperty = LastFileNamePropertyKey.DependencyProperty;

        [Configurable("")]
        public string LastFileName
        {
            get
            {
                return (string)GetValue(LastFileNameProperty);
            }
            private set
            {
                SetValue(LastFileNamePropertyKey, value);
            }
        }
        
        public static readonly DependencyProperty TemplateFileProperty = DependencyProperty.Register(
            "TemplateFile", typeof(string), typeof(AvalonEditor), new FrameworkPropertyMetadata("template.nut")
        );

        [Configurable("../examples/template.nut")]
        public string TemplateFile
        {
            get
            {
                return (string)GetValue(TemplateFileProperty);
            }
            set
            {
                SetValue(TemplateFileProperty, value);
            }
        }

        public static readonly DependencyPropertyKey IsUnsavedPropertyKey = DependencyProperty.RegisterReadOnly(
            "IsUnsaved", typeof(bool), typeof(AvalonEditor), new PropertyMetadata(false));

        public static readonly DependencyProperty IsUnsavedProperty = IsUnsavedPropertyKey.DependencyProperty;

        public bool IsUnsaved
        {
            get
            {
                return (bool) GetValue(IsUnsavedProperty);
            }
            private set
            {
                SetValue(IsUnsavedPropertyKey, value);
            }
        }

        public static readonly DependencyProperty CaretLineProperty = DependencyProperty.Register(
            "CaretLine", typeof(int), typeof(AvalonEditor), new FrameworkPropertyMetadata(0, new PropertyChangedCallback(OnCaretLineChanged))
        );

        public int CaretLine
        {
            get
            {
                return (int)GetValue(CaretLineProperty);
            }
            set
            {
                SetValue(CaretLineProperty, value);
            }
        }

        private static void OnCaretLineChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            ((AvalonEditor)d).editor.TextArea.Caret.Line = ((AvalonEditor)d).CaretLine;
        }

        public static readonly DependencyProperty CaretColumnProperty = DependencyProperty.Register(
            "CaretColumn", typeof(int), typeof(AvalonEditor), new FrameworkPropertyMetadata(0, new PropertyChangedCallback(OnCaretColumnChanged))
        );

        public int CaretColumn
        {
            get
            {
                return (int)GetValue(CaretColumnProperty);
            }
            set
            {
                SetValue(CaretColumnProperty, value);
            }
        }

        private static void OnCaretColumnChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            ((AvalonEditor)d).editor.TextArea.Caret.Column = ((AvalonEditor)d).CaretColumn;
        }

        public static readonly DependencyProperty CaretOffsetProperty = DependencyProperty.Register(
            "CaretOffset", typeof(int), typeof(AvalonEditor), new FrameworkPropertyMetadata(0, new PropertyChangedCallback(OnCaretOffsetChanged))
        );

        public int CaretOffset
        {
            get
            {
                return (int)GetValue(CaretOffsetProperty);
            }
            set
            {
                SetValue(CaretOffsetProperty, value);
            }
        }

        private static void OnCaretOffsetChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            ((AvalonEditor)d).editor.TextArea.Caret.Offset = ((AvalonEditor)d).CaretOffset;
        }

        public static readonly DependencyProperty SelectionStartProperty = DependencyProperty.Register(
            "SelectionStart", typeof(int), typeof(AvalonEditor), new FrameworkPropertyMetadata(0, new PropertyChangedCallback(OnSelectionStartChanged))
        );

        public int SelectionStart
        {
            get
            {
                return (int)GetValue(SelectionStartProperty);
            }
            set
            {
                SetValue(SelectionStartProperty, value);
            }
        }

        private static void OnSelectionStartChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            AvalonEditor editor = (AvalonEditor)d;

            if(editor.enablePropertyCallbacks) editor.editor.SelectionStart = editor.SelectionStart;
        }

        public static readonly DependencyProperty SelectionEndProperty = DependencyProperty.Register(
            "SelectionEnd", typeof(int), typeof(AvalonEditor), new FrameworkPropertyMetadata(0, new PropertyChangedCallback(OnSelectionEndChanged))
            );

        public int SelectionEnd
        {
            get
            {
                return (int)GetValue(SelectionEndProperty);
            }
            set
            {
                SetValue(SelectionEndProperty, value);
            }
        }

        private static void OnSelectionEndChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            AvalonEditor editor = (AvalonEditor)d;

            if (editor.enablePropertyCallbacks) editor.editor.SelectionLength = editor.SelectionEnd - editor.SelectionStart;
        }

        public static readonly DependencyProperty SelectionLengthProperty = DependencyProperty.Register(
            "SelectionLength", typeof(int), typeof(AvalonEditor), new FrameworkPropertyMetadata(0, new PropertyChangedCallback(OnSelectionLengthChanged))
            );

        public int SelectionLength
        {
            get
            {
                return (int)GetValue(SelectionLengthProperty);
            }
            set
            {
                SetValue(SelectionLengthProperty, value);
            }
        }

        private static void OnSelectionLengthChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            AvalonEditor editor = (AvalonEditor)d;
            if (editor.enablePropertyCallbacks) editor.editor.SelectionLength = editor.SelectionLength;
        }

        public static readonly DependencyProperty DefaultFontSizeProperty = DependencyProperty.Register(
            "DefaultFontSize", typeof(int), typeof(AvalonEditor), new FrameworkPropertyMetadata(13)
        );

        [Configurable(13)]
        public int DefaultFontSize
        {
            get
            {
                return (int)GetValue(DefaultFontSizeProperty);
            }
            set
            {
                SetValue(DefaultFontSizeProperty, value);
            }
        }

        
        public static readonly DependencyProperty WordWrapProperty = DependencyProperty.Register(
            "WordWrap", typeof(bool), typeof(AvalonEditor), new FrameworkPropertyMetadata(false, new PropertyChangedCallback(OnWordWrapChanged))
        );

        [Configurable(false)]
        public bool WordWrap
        {
            get
            {
                return (bool)GetValue(WordWrapProperty);
            }
            set
            {
                SetValue(WordWrapProperty, value);
            }
        }

        private static void OnWordWrapChanged(DependencyObject d, DependencyPropertyChangedEventArgs e){
            ((AvalonEditor)d).editor.WordWrap = ((AvalonEditor)d).WordWrap;
        }
        #endregion

        #region Commands
        private ICommand newCommand;

        public ICommand NewCommand
        {
            get
            {
                if (this.newCommand == null)
                {
                    this.newCommand = new RelayCommand(param => this.New());
                }

                return newCommand;
            }
        }

        private ICommand openCommand;

        public ICommand OpenCommand
        {
            get
            {
                if (this.openCommand == null)
                {
                    this.openCommand = new RelayCommand(param => this.Open());
                }

                return openCommand;
            }
        }

        private ICommand saveCommand;

        public ICommand SaveCommand
        {
            get
            {
                if (this.saveCommand == null)
                {
                    this.saveCommand = new RelayCommand(param => this.Save());
                }

                return saveCommand;
            }
        }

        private ICommand saveAsCommand;

        public ICommand SaveAsCommand
        {
            get
            {
                if (this.saveAsCommand == null)
                {
                    this.saveAsCommand = new RelayCommand(param => this.SaveAs());
                }

                return saveAsCommand;
            }
        }

        private ICommand increaseFontSizeCommand;

        public ICommand IncreaseFontSizeCommand
        {
            get
            {
                if (this.increaseFontSizeCommand == null)
                {
                    this.increaseFontSizeCommand = new RelayCommand(param => ++this.FontSize);
                }

                return increaseFontSizeCommand;
            }
        }

        private ICommand decreaseFontSizeCommand;

        public ICommand DecreaseFontSizeCommand
        {
            get
            {
                if (this.decreaseFontSizeCommand == null)
                {
                    this.decreaseFontSizeCommand = new RelayCommand(param => --this.FontSize);
                }

                return decreaseFontSizeCommand;
            }
        }

        private ICommand resetFontSizeCommand;

        public ICommand ResetFontSizeCommand
        {
            get
            {
                if (this.resetFontSizeCommand == null)
                {
                    this.resetFontSizeCommand = new RelayCommand(param => this.FontSize = this.DefaultFontSize);
                }

                return resetFontSizeCommand;
            }
        }

        private ICommand toggleWordWrapCommand;

        public ICommand ToggleWordWrapCommand
        {
            get
            {
                if (this.toggleWordWrapCommand == null)
                {
                    this.toggleWordWrapCommand = new RelayCommand(param => this.WordWrap = !this.WordWrap);
                }

                return toggleWordWrapCommand;
            }
        }

        private ICommand undoCommand;

        public ICommand UndoCommand
        {
            get
            {
                if (this.undoCommand == null)
                {
                    this.undoCommand = new RelayCommand(param => this.editor.Undo());
                }

                return undoCommand;
            }
        }

        private ICommand redoCommand;

        public ICommand RedoCommand
        {
            get
            {
                if (this.redoCommand == null)
                {
                    this.redoCommand = new RelayCommand(param => this.editor.Redo());
                }

                return redoCommand;
            }
        }
#endregion

        private bool enablePropertyCallbacks = true;
        private ICSharpCode.AvalonEdit.Folding.AbstractFoldingStrategy foldingStrategy;
        private ICSharpCode.AvalonEdit.Folding.FoldingManager foldingManager;

        public AvalonEditor()
        {                        
            InitializeComponent();
            MainConfig.Register(this);
        }
       
        public void Register(IDockManager dockManager)
        {
            dockManager.AddAsDocumentContent(this, "Code", true);
        }

        /// <summary>
        /// Registers window-wide hotkeys.
        /// </summary>
        /// <param name="mainWindow">The main window.</param>
        public void Register(IMainWindow mainWindow)
        {
            mainWindow.RegisterInputGesture(
                new KeyGesture(Key.N, ModifierKeys.Control),
                this.NewCommand
            );

            mainWindow.RegisterInputGesture(
                new KeyGesture(Key.O, ModifierKeys.Control),
                this.OpenCommand
            );

            mainWindow.RegisterInputGesture(
                new KeyGesture(Key.S, ModifierKeys.Control),
                this.SaveCommand
            );

            mainWindow.RegisterInputGesture(
                new KeyGesture(Key.S, ModifierKeys.Control | ModifierKeys.Shift),
                this.SaveAsCommand
            );
        }

        /// <summary>
        /// Registers the editor into main window menu.
        /// </summary>
        /// <param name="menuBar">The menu bar.</param>
        public void Register(IMenuBar menuBar){
            string iconPathPrefix = "pack://application:,,,/GGenStudio.PlugIn.AvalonEditor;component/Images/Icons/";

            menuBar.AddMenu(
                new MenuEntry(
                    header: "File",
                    items: new MenuEntryObservableCollection()
                    {
                        new MenuEntry(
                            header: "New",
                            priority: 0,
                            command: this.NewCommand,
                            inputGestureText: "Ctrl+N",
                            icon: iconPathPrefix + "new.png"
                        ),
                        new MenuEntry(
                            header: "Open",
                            priority: -1,
                            command: this.OpenCommand,
                            inputGestureText: "Ctrl+O",
                            icon: iconPathPrefix + "open.png"
                        ),
                        new MenuEntry(
                            header: "Save",
                            priority: -2,
                            command: this.SaveCommand,
                            inputGestureText: "Ctrl+S",
                            icon: iconPathPrefix + "save.png"
                        ),
                        new MenuEntry(
                            header: "Save As...",
                            priority: -3,
                            command: this.SaveAsCommand,
                            inputGestureText: "Ctrl+Shift+S",
                            icon: iconPathPrefix + "save.png"
                        ),
                        new MenuSeparator(
                            priority: -4
                        )
                    }
                )
            );

            menuBar.AddMenu(
                new MenuEntry(
                    header: "Edit",
                    priority: -1,
                    items: new MenuEntryObservableCollection()
                    {
                        new MenuEntry(
                            header: "Undo",
                            priority: 0,
                            command: this.UndoCommand,
                            inputGestureText: "Ctrl+Z",
                            icon: iconPathPrefix + "undo.png"
                        ),
                        new MenuEntry(
                            header: "Redo",
                            priority: -1,
                            command: this.RedoCommand,
                            inputGestureText: "Ctrl+Shift+Z",
                            icon: iconPathPrefix + "redo.png"
                        ),
                        new MenuSeparator(
                            priority: -2
                        ),
                        new MenuEntry(
                            header: "Cut",
                            priority: -3,
                            command: ApplicationCommands.Cut,
                            inputGestureText: "Ctrl+X",
                            icon: iconPathPrefix + "cut.png"
                        ),
                        new MenuEntry(
                            header: "Copy",
                            priority: -4,
                            command: ApplicationCommands.Copy,
                            inputGestureText: "Ctrl+C",
                            icon: iconPathPrefix + "copy.png"
                        ),
                        new MenuEntry(
                            header: "Paste",
                            priority: -5,
                            command: ApplicationCommands.Paste,
                            inputGestureText: "Ctrl+P",
                            icon: iconPathPrefix + "paste.png"
                        )
                    }
                )
            );

            menuBar.AddMenu(
                new MenuEntry(
                    header: "View",
                    priority: -2,
                    items: new MenuEntryObservableCollection()
                    {
                        new MenuEntry(
                            header: "Code",
                            priority: 0,
                            command: null,
                            icon: iconPathPrefix + "code.png"
                        ),
                        new MenuSeparator(
                            priority: -20
                        ),
                        new MenuEntry(
                            header: "Word Wrap",
                            priority: -21,
                            command: this.ToggleWordWrapCommand,
                            isCheckable: true,
                            dataContext: this,
                            isCheckedBinding: new Binding("WordWrap")
                        ),
                        new MenuSeparator(
                            priority: -22
                        ),
                        new MenuEntry(
                            header: "Increase Font Size",
                            priority: -23,
                            command: this.IncreaseFontSizeCommand
                        ),
                        new MenuEntry(
                            header: "Decrease Font Size",
                            priority: -24,
                            command: this.DecreaseFontSizeCommand
                        ),
                        new MenuEntry(
                            header: "Reset Font Size",
                            priority: -25,
                            command: this.ResetFontSizeCommand
                        ),
                    }
                )
            );
        }

        private void editor_Loaded(object sender, RoutedEventArgs e)
        {
            this.editor.TextArea.SnapsToDevicePixels = true;
            this.IsUnsaved = false;
            this.InitializeSyntaxHighlighing();

            // Code folding
            this.foldingManager = ICSharpCode.AvalonEdit.Folding.FoldingManager.Install(editor.TextArea);
            this.foldingStrategy = new BraceFoldingStrategy();
            this.foldingStrategy.UpdateFoldings(this.foldingManager, this.editor.Document);

            // window might not always be available (for example in designer)
            try{
                // we want to ask the user if 
                Window.GetWindow(this).Closing += new System.ComponentModel.CancelEventHandler(Window_Closing);
            }
            catch (NullReferenceException) { };
            
            this.editor.TextChanged += new System.EventHandler(editor_TextChanged);
            this.editor.TextArea.TextEntered += new System.Windows.Input.TextCompositionEventHandler(editor_TextChanged);
            this.editor.TextArea.SelectionChanged += new System.EventHandler(editor_SelectionChanged);
            this.editor.TextArea.Caret.PositionChanged += new System.EventHandler(editor_SelectionChanged);                

            this.New();
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if(this.IsUnsaved){

                MessageBoxResult result = MessageBox.Show("Do you wish to save unsaved content?", "", MessageBoxButton.YesNoCancel);                

                switch(result){
                    case MessageBoxResult.Yes:
                        try
                        {
                            this.Save();
                        }
                        catch(Exception){
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

        private void InitializeSyntaxHighlighing(){
            try
            {
                using (System.Xml.XmlTextReader reader = new System.Xml.XmlTextReader("squirrel.xshd"))
                {
                    this.editor.SyntaxHighlighting = ICSharpCode.AvalonEdit.Highlighting.Xshd.HighlightingLoader.Load(reader, ICSharpCode.AvalonEdit.Highlighting.HighlightingManager.Instance);                
                }
            }
            catch (System.IO.FileNotFoundException)
            {
                this.ThrowMessage(new Message("Could not read syntax highlighter definitions file.", MessageType.Warning));
            }
        }

        private void editor_TextChanged(object sender, EventArgs e)
        {
            this.IsUnsaved = true;

            this.enablePropertyCallbacks = false;

            this.Text = this.editor.Text;

            this.enablePropertyCallbacks = true;
            this.foldingStrategy.UpdateFoldings(this.foldingManager, this.editor.Document);
            //this.ScheduleSyntaxCheck;
        }

        private void editor_SelectionChanged(object sender, EventArgs e)
        {
            this.enablePropertyCallbacks = false;

            this.SelectionStart = this.editor.SelectionStart;
            this.SelectionEnd = this.editor.SelectionStart + this.editor.SelectionLength;
            this.SelectionLength = this.editor.SelectionLength;
            this.CaretColumn = this.editor.TextArea.Caret.Column;
            this.CaretLine = this.editor.TextArea.Caret.Line;

            this.enablePropertyCallbacks = true;
        }

        public void New()
        {
            if(TemplateFile == ""){
                this.editor.Text = "";
            }
            else{
                try
                {                
                    this.editor.Text = System.IO.File.ReadAllText(this.TemplateFile);
                }
                catch {
                    this.ThrowMessage(new Message("Could not open template file.", MessageType.Warning));
                };
            }
            
            this.CurrentFileName = "";
            this.IsUnsaved = false;
        }

        public void Save()
        {
            try
            {
                if(this.CurrentFileName == "") this.CurrentFileName = this.LastFileName = Utilities.FileDialog.ShowSave(this.LastFileName, "Squirrel Scripts (*.nut)|*.nut|All files|*.*");

                File.WriteAllText(this.CurrentFileName, this.editor.Text);

                this.IsUnsaved = false;
            }
            catch { };
        }

        public void SaveAs()
        {           
            try{
                this.CurrentFileName = this.LastFileName = Utilities.FileDialog.ShowSave(this.LastFileName, "Squirrel Scripts (*.nut)|*.nut|All files|*.*");
                
                File.WriteAllText(this.CurrentFileName, this.editor.Text);

                this.IsUnsaved = false;                
            }
            catch { };            
        }

        public void Open()
        {
            try
            {
                this.CurrentFileName = this.LastFileName = Utilities.FileDialog.ShowOpen(this.LastFileName, "Squirrel Scripts (*.nut)|*.nut|All files|*.*");

                this.editor.Text = File.ReadAllText(this.CurrentFileName);

                this.IsUnsaved = false;
            }
            catch {
                this.ThrowMessage(new Message("File \"" + this.LastFileName + "\" is not readable", MessageType.Error));
            };
        }
        
        public void ReplaceString(int startOffset, int length, string replace){
            this.editor.Document.Replace(startOffset, length, replace);
        }        
    }
}