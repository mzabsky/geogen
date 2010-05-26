using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Xml.Serialization;
using System.Windows.Media;
using System.Windows.Interop;
using System.Windows.Documents;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Forms.Integration;
using ICSharpCode.AvalonEdit.Utils;
using System.Windows.Controls.Primitives;

namespace GeoGen_Studio
{
    public class GGenMethod : ICSharpCode.AvalonEdit.CodeCompletion.ICompletionData
    {
        [XmlElement(ElementName = "Class")]
        public string className;

        [XmlElement(ElementName = "Name")]
        public string methodName;

        [XmlElement(ElementName = "Type")]
        public string returnTypeName;

        [XmlElement(ElementName = "Description")]
        public string description;

        [XmlElement(ElementName = "Param")]
        public List<GGenParam> parameters;

        public GGenMethod()
        {
        }

        [XmlIgnoreAttribute]
        public System.Windows.Media.ImageSource Image
        {
            get { return null; }
        }

        [XmlIgnoreAttribute]
        public string Text
        {
            get
            {
                return this.methodName;
            }
            private set
            {
                this.methodName = value;
            }
        }

        // Use this property if you want to show a fancy UIElement in the list.
        [XmlIgnoreAttribute]
        public object Content
        {
            get { return this.methodName; }
        }

        [XmlIgnoreAttribute]
        public double Priority
        {
            get { return 0; }
        }

        [XmlIgnoreAttribute]
        public object Description
        {
            get {
                /*string str = this.returnTypeName;

                if (this.className != "") str += className + "::";

                str += methodName + "(";

                foreach(GGenParam param in this.parameters){
                    str += param.typeName + " " + param.paramName + ", ";
                }

                str += ")\n" + this.description;

                System.Windows.Controls.RichTextBox rtf = new System.Windows.Controls.RichTextBox();
                rtf.BorderBrush = null;
                rtf.Background = null;
            
            
                rtf.Document = new FlowDocument();

                Paragraph para1 = new Paragraph();

                Run run1 = new Run(this.returnTypeName);
                run1.Foreground = System.Windows.Media.Brushes.Teal;
                para1.Inlines.Add(run1);

                para1.Inlines.Add(" ");

                if (this.className != "") para1.Inlines.Add(className + "::");

                para1.Inlines.Add(new Bold(new Run(this.methodName)));

                para1.Inlines.Add("(");

                //if (this.parameters.Count > 0) para1.Inlines.Add(new LineBreak());

                Table table = new Table();
                TableRowGroup rowGroup = new TableRowGroup();

                bool first = true;
                foreach (GGenParam param in this.parameters)
                {
                    Paragraph cell1Para = new Paragraph();
                    Run paramTypeRun = new Run(param.typeName);
                    paramTypeRun.Foreground = System.Windows.Media.Brushes.Teal;
                    cell1Para.Inlines.Add(paramTypeRun);

                    Paragraph cell2Para = new Paragraph();
                    cell2Para.Inlines.Add(param.paramName);

                    Paragraph cell3Para = new Paragraph();
                    Run paramDescRun = new Run(param.description);
                    paramDescRun.Foreground = System.Windows.Media.Brushes.Green;
                    cell3Para.Inlines.Add(paramDescRun);*/
                    /*if(first){
                        first = false;
                    }
                    else{
                        para1.Inlines.Add(", ");       
                    }*/

                    //para1.Inlines.Add(new LineBreak());

                    /*

                    //para1.Inlines.Add(" " + param.paramName + "   ");


                    TableCell cell1 = new TableCell(cell1Para);
                    cell1.TextAlignment = TextAlignment.Right;
                    cell1.

                    TableRow row = new TableRow();
                    row.Cells.Add(cell1);
                    row.Cells.Add(new TableCell(cell2Para));
                    row.Cells.Add(new TableCell(cell3Para));


                    rowGroup.Rows.Add(row);
                }
                table.RowGroups.Add(rowGroup);

                //para1.Inlines.Add(new LineBreak());
                //para1.Inlines.Add(")");
                para1.FontFamily = new System.Windows.Media.FontFamily("Consolas, Courier New");
                para1.TextIndent = - 30;
                para1.Margin = new System.Windows.Thickness(30, 0, 0, 0);    

                rtf.Document.Blocks.Add(para1);
                rtf.Document.Blocks.Add(table);
                rtf.Document.Blocks.Add(new Paragraph(new Run(this.description)));

                rtf.Width = 800;

                //rtf;

    */

                System.Windows.Controls.StackPanel panel = new System.Windows.Controls.StackPanel();

                System.Windows.Controls.RichTextBox rtf = new System.Windows.Controls.RichTextBox();
                rtf.BorderBrush = null;
                rtf.Background = null;
                rtf.HorizontalAlignment = HorizontalAlignment.Left;

                Paragraph para1 = new Paragraph();
                Run run1 = new Run(this.returnTypeName);
                run1.Foreground = System.Windows.Media.Brushes.Teal;
                para1.Inlines.Add(run1);

                para1.Inlines.Add(" ");

                if (this.className != "") para1.Inlines.Add(className + "::");

                para1.Inlines.Add(new Bold(new Run(this.methodName)));

                para1.Inlines.Add(" (");

                para1.Padding = new System.Windows.Thickness(0, 0, 0, 0);
                para1.Margin = new System.Windows.Thickness(0, 0, 0, 0);

                rtf.Document = new FlowDocument();
                rtf.Document.Blocks.Add(para1);
                rtf.Width = 300;
                rtf.Padding = new System.Windows.Thickness(0, 0, 0, 0);
                rtf.Margin = new System.Windows.Thickness(0, 0, 0, 0);
                rtf.FontFamily = new System.Windows.Media.FontFamily("Consolas, Courier New");

                if(this.parameters.Count == 0){
                    para1.Inlines.Add(")");

                    panel.Children.Add(rtf);
                }
                else{
                    panel.Children.Add(rtf);

                    StackPanel rows = new StackPanel();
                    rows.Orientation = Orientation.Vertical;
                    rows.Margin = new System.Windows.Thickness(10, 0, 0, 0);

                    foreach (GGenParam param in this.parameters)
                    {
                        StackPanel row = new StackPanel();
                        row.Orientation = Orientation.Horizontal;
                        row.Margin = new System.Windows.Thickness(0, 0, 0, 0);

                        TextBlock paramType = new TextBlock();
                        paramType.Text = param.typeName;
                        paramType.Foreground = System.Windows.Media.Brushes.Teal;
                        paramType.Width = 100;
                        paramType.Padding = new System.Windows.Thickness(1, 1, 0, 1);
                        paramType.TextAlignment = TextAlignment.Right;
                        paramType.FontFamily = new System.Windows.Media.FontFamily("Consolas, Courier New");

                        row.Children.Add(paramType);

                        TextBlock paramName = new TextBlock();
                        paramName.Text = param.paramName;
                        paramName.Width = 80;
                        paramName.Padding = new System.Windows.Thickness(5, 1, 1, 1);
                        paramName.FontFamily = new System.Windows.Media.FontFamily("Consolas, Courier New");

                        row.Children.Add(paramName);

                        TextBlock paramDesc = new TextBlock();
                        paramDesc.Text = param.description;
                        paramDesc.Foreground = System.Windows.Media.Brushes.Green;
                        paramDesc.Padding = new System.Windows.Thickness(1, 1, 1, 1);
                        paramDesc.TextWrapping = TextWrapping.Wrap;
                        paramDesc.MaxWidth = 210;
                        paramDesc.FontFamily = new System.Windows.Media.FontFamily("Consolas, Courier New");

                        row.Children.Add(paramDesc);

                        rows.Children.Add(row);
                    }

                    panel.Children.Add(rows);

                    TextBlock bracket = new TextBlock();
                    bracket.Text = ")";
                    bracket.Padding = new System.Windows.Thickness(7, 0, 0, 0);
                    bracket.FontFamily = new System.Windows.Media.FontFamily("Consolas, Courier New");

                    panel.Children.Add(bracket);
                }

                TextBlock desc = new TextBlock();
                desc.Text = this.description;
                desc.Padding = new System.Windows.Thickness(7, 8, 0, 0);
                desc.TextWrapping = TextWrapping.Wrap;

                panel.Children.Add(desc);

                panel.MaxWidth = 600;

                //
                //panel.Children.Add(label2);

                return panel; 
            }
        }

        public void Complete(ICSharpCode.AvalonEdit.Editing.TextArea textArea, ICSharpCode.AvalonEdit.Document.ISegment completionSegment,
            EventArgs insertionRequestEventArgs)
        {
            textArea.Document.Replace(completionSegment, this.Text);
        }
    }

    public class GGenParam
    {
        [XmlElement(ElementName = "Name")]
        public string paramName;

        [XmlElement(ElementName = "Type")]
        public string typeName;

        [XmlElement(ElementName = "Description")]
        public string description;
    }

    public class GGenConstant : ICSharpCode.AvalonEdit.CodeCompletion.ICompletionData
    {
        [XmlElement(ElementName = "Name")]
        public string constantName;

        [XmlElement(ElementName = "Type")]
        public string typeName;

        [XmlElement(ElementName = "Description")]
        public string description;

        public GGenConstant()
        {
        }

        [XmlIgnoreAttribute]
        public System.Windows.Media.ImageSource Image
        {
            get { return null; }
        }

        [XmlIgnoreAttribute]
        public string Text
        {
            get
            {
                return this.constantName;
            }
            private set
            {
                this.constantName = value;
            }
        }

        // Use this property if you want to show a fancy UIElement in the list.
        [XmlIgnoreAttribute]
        public object Content
        {
            get { return this.constantName; }
        }

        [XmlIgnoreAttribute]
        public double Priority
        {
            get { return 0; }
        }

        [XmlIgnoreAttribute]
        public object Description
        {
            get {
                System.Windows.Controls.StackPanel panel = new System.Windows.Controls.StackPanel();

                System.Windows.Controls.RichTextBox rtf = new System.Windows.Controls.RichTextBox();
                rtf.BorderBrush = null;
                rtf.Background = null;
                rtf.HorizontalAlignment = HorizontalAlignment.Left;

                Paragraph para1 = new Paragraph();
                Run run1 = new Run(this.typeName);
                run1.Foreground = System.Windows.Media.Brushes.Teal;
                para1.Inlines.Add(run1);

                para1.Inlines.Add(" ");

                para1.Inlines.Add(new Bold(new Run(this.constantName)));

                para1.Padding = new System.Windows.Thickness(0, 0, 0, 0);
                para1.Margin = new System.Windows.Thickness(0, 0, 0, 0);

                rtf.Document = new FlowDocument();
                rtf.Document.Blocks.Add(para1);
                rtf.Width = 300;
                rtf.Padding = new System.Windows.Thickness(0, 0, 0, 0);
                rtf.Margin = new System.Windows.Thickness(0, 0, 0, 0);

                panel.Children.Add(rtf);

                TextBlock desc = new TextBlock();
                desc.Text = this.description;
                desc.Padding = new System.Windows.Thickness(7, 0, 0, 0);
                desc.TextWrapping = TextWrapping.Wrap;

                panel.Children.Add(desc);

                panel.MaxWidth = 420;

                return panel;         
            }
        }

        public void Complete(ICSharpCode.AvalonEdit.Editing.TextArea textArea, ICSharpCode.AvalonEdit.Document.ISegment completionSegment,
            EventArgs insertionRequestEventArgs)
        {
            textArea.Document.Replace(completionSegment, this.Text);
        }
    }

    [XmlRoot("APIData")]
    public class GGenAPIData
    {
        [XmlElement(ElementName = "Method")]
        public List<GGenMethod> methods;

        [XmlElement(ElementName = "Constant")]
        public List<GGenConstant> constants;
    }

    class CompletionDataComparer : IComparer<ICSharpCode.AvalonEdit.CodeCompletion.ICompletionData>
    {
        public int Compare(ICSharpCode.AvalonEdit.CodeCompletion.ICompletionData a, ICSharpCode.AvalonEdit.CodeCompletion.ICompletionData b)
        {
            int returnValue = 1;
            if (a != null && b != null)
            {
                returnValue = a.Text.CompareTo(b.Text);
            }

            return returnValue;
        }
    }

    public class CompletionListItem: TextBlock{
        public ICSharpCode.AvalonEdit.CodeCompletion.ICompletionData original;

        public bool ShowIfMatching(string str){
            this.Visibility = this.Text.StartsWith(str, System.StringComparison.OrdinalIgnoreCase) ? Visibility.Visible : Visibility.Collapsed;
            return this.Visibility == Visibility.Visible;
        }
    }

    public class CompletionWindow: ElementHost{
        public enum State
        {
            Accepted,
            Closed,
            Open
        };

        public CompletionListItem selectedItem;
        string str;
        UIElementCollection items;
        public ScrollViewer scroller;
        Border border2 = new Border();
        public State state;

        public string Str
        {
            get { return str; }
            set { str = value;
                int numVisible = 0;
                foreach(UIElement element in items){
                    CompletionListItem item = (CompletionListItem)element;

                    if(item.ShowIfMatching(str)) numVisible++;
                }

                this.SelectFirstVisible();

                if (numVisible == 0) Main.Get().CloseCompletionWindow();
                else if (numVisible == 1) Main.Get().AcceptCompletion();

                if(selectedItem.Visibility == Visibility.Collapsed){
                    this.SelectFirstVisible();
                }
            }
        }

        public string SelectedText
        {
            get { return this.selectedItem.Text; }
        }

        public CompletionWindow(){
            StackPanel outerPanel = new StackPanel();
            outerPanel.Orientation = Orientation.Horizontal;

            Border border = new Border();
            this.scroller = new ScrollViewer();
            StackPanel panel = new StackPanel();
            Main main = Main.Get();
            items = panel.Children;
            
            panel.Background = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Color.FromRgb((byte)246, (byte)246, (byte)246));

            this.Width = 600;
            this.Height = 230;
            //this.Topmost = true;

            foreach(ICSharpCode.AvalonEdit.CodeCompletion.ICompletionData item in main.completionData){
                CompletionListItem element = new CompletionListItem();
                element.Text = item.Text;
                element.original = item;
                element.Background = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Color.FromRgb((byte)246, (byte)246, (byte)246));
                element.FontSize = 13;

                element.PreviewMouseUp += delegate(object o, System.Windows.Input.MouseButtonEventArgs args)
                {
                    if(args.ChangedButton == System.Windows.Input.MouseButton.Left){
                        if (o == this.selectedItem)
                        {
                            Main.Get().AcceptCompletion();
                            return;
                        }
                        
                        SelectItem((CompletionListItem) o);
                    }

                    main.editor.Focus();
                };

                element.Padding = new System.Windows.Thickness(2, 2, 2, 2);
                element.FontFamily = new System.Windows.Media.FontFamily("Consolas, Courier New");

                panel.Children.Add(element);
            }

            this.scroller.Content = panel;
            border.BorderThickness = new System.Windows.Thickness(1, 1, 1, 1);
            border.BorderBrush = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Color.FromRgb((byte) 118, (byte) 118, (byte) 118));
            border.CornerRadius = new System.Windows.CornerRadius(2, 2, 2, 2);
            border.Width = 180;

            border.Child = this.scroller;

            outerPanel.Children.Add(border);

            border2.BorderThickness = new System.Windows.Thickness(0, 1, 1, 1);
            border2.BorderBrush = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Color.FromRgb((byte) 118, (byte) 118, (byte) 118));
            border2.CornerRadius = new System.Windows.CornerRadius(2, 2, 2, 2);
            border2.Width = 420;
            border2.Background = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Color.FromRgb((byte)246, (byte)246, (byte)246));

            outerPanel.Children.Add(border2);

            outerPanel.Background = System.Windows.Media.Brushes.White;
            //border2.

            this.BackColor = System.Drawing.Color.FromKnownColor(System.Drawing.KnownColor.White);
            this.Child = outerPanel;

            this.SelectFirstVisible();
        }

        public void SelectItem(CompletionListItem item){
            if (selectedItem != null) selectedItem.Background = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Color.FromRgb((byte)246, (byte)246, (byte)246));
            this.selectedItem = item;
            this.selectedItem.Background = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Color.FromRgb((byte)112, (byte)183, (byte)255));
            //this.selectedItem.Corner
            //this.toolTip.Content = item.original.Description;
            this.border2.Child = (UIElement) item.original.Description;

            this.scroller.ScrollToVerticalOffset(this.selectedItem.ActualHeight * items.IndexOf(item) - this.Height / 2);
        }

        public void SelectNextItem(){
            if (selectedItem == null) SelectFirstVisible();

            int index = items.IndexOf(selectedItem);

            index++;

            while(index < items.Count){
                if(items[index].Visibility == Visibility.Visible){
                    SelectItem((CompletionListItem) items[index]);
                    break;
                }

                index++;
            }
        }

        public void SelectPrevItem()
        {
            if (selectedItem == null) SelectFirstVisible();

            int index = items.IndexOf(selectedItem);

            index--;

            while (index >= 0)
            {
                if (items[index].Visibility == Visibility.Visible)
                {
                    SelectItem((CompletionListItem)items[index]);
                    break;
                }

                index--;
            }
        }

        public void SelectFirstVisible(){
            foreach (UIElement element in items)
            {
                if(element.Visibility == Visibility.Visible){
                    this.SelectItem((CompletionListItem)element);

                    return;
                }
            }
        }

        /*public void UpdatePosition(){
            Main main = Main.Get();

            //this.SetPosition();
        }*/

        /*protected void SetPosition(TextViewPosition position)
        {
            TextView textView = this.TextArea.TextView;


            UpdatePosition();
        }*/

        public void UpdatePosition()
        {
            Main main = Main.Get();

            ICSharpCode.AvalonEdit.Rendering.TextView textView = main.editor.TextArea.TextView;
            ICSharpCode.AvalonEdit.TextViewPosition position = new ICSharpCode.AvalonEdit.TextViewPosition(main.editor.Document.GetLocation(main.completionStartPosition));

            Point visualLocation = textView.GetVisualPosition(position, ICSharpCode.AvalonEdit.Rendering.VisualYPosition.LineBottom);
            Point visualLocationTop = textView.GetVisualPosition(position, ICSharpCode.AvalonEdit.Rendering.VisualYPosition.LineTop);

            // PointToScreen returns device dependent units (physical pixels)
            Point location = textView.PointToScreen(visualLocation - textView.ScrollOffset);
            Point locationTop = textView.PointToScreen(visualLocationTop - textView.ScrollOffset);

            // Let's use device dependent units for everything
            //Size completionWindowSize = new Size(this.Width, this.Height).TransformToDevice(textView);
            Rect bounds = new Rect(location, new Size(this.Width, this.Height));
            
            System.Drawing.Rectangle systemRect = System.Windows.Forms.Screen.GetWorkingArea(new System.Drawing.Point((int) location.X, (int) location.Y));
            Rect workingScreen = new Rect(new Point(systemRect.Location.X,systemRect.Location.Y), new Size(systemRect.Size.Width, systemRect.Size.Height));//.ToWpf();
            if (!workingScreen.Contains(bounds))
            {
                if (bounds.Left < workingScreen.Left)
                {
                    bounds.X = workingScreen.Left;
                }
                else if (bounds.Right > workingScreen.Right)
                {
                    bounds.X = workingScreen.Right - bounds.Width;
                }
                if (bounds.Bottom > workingScreen.Bottom)
                {
                    bounds.Y = locationTop.Y - bounds.Height;
                }
                if (bounds.Y < workingScreen.Top)
                {
                    bounds.Y = workingScreen.Top;
                }
            }
            // Convert the window bounds to device independent units

            Matrix matrix = PresentationSource.FromVisual(textView).CompositionTarget.TransformFromDevice;

            bounds = Rect.Transform(bounds, matrix);
            this.Left = (int) bounds.X - 10 - main.Left;
            this.Top = (int)bounds.Y - 27 - main.Top;
        }
    }

    public partial class Main : System.Windows.Forms.Form
    {
        public CompletionWindow completionWindow = null;
        public List<ICSharpCode.AvalonEdit.CodeCompletion.ICompletionData> completionData = new List<ICSharpCode.AvalonEdit.CodeCompletion.ICompletionData>();
        public int completionStartPosition;

        void RegisterCompletionEvents(){
            editor.TextArea.TextEntered += delegate(object s, System.Windows.Input.TextCompositionEventArgs args)
            {
                this.needsSaving = true;

                //System.Windows.Input.TextCompositionEventArgs args = (System.Windows.Input.TextCompositionEventArgs)args_o;

                if (args.Text == ".")
                {
                    this.ShowCompletionWindow("");
                }
            };

            editor.TextArea.TextEntering += delegate(object s, System.Windows.Input.TextCompositionEventArgs args)
            {
                /*if (args.Text.Length > 0 && completionWindow != null)
                {
                    if (!char.IsLetterOrDigit(args.Text[0]))
                    {
                        // Whenever a non-letter is typed while the completion window is open,
                        // insert the currently selected element.
                        completionWindow.CompletionList.RequestInsertion(args);
                    }
                }*/
            };

            editor.PreviewKeyDown += delegate(object s, System.Windows.Input.KeyEventArgs args)
            {
                System.Windows.Input.KeyConverter converter = new System.Windows.Input.KeyConverter();
                string code = converter.ConvertToString(args.Key);
                
                if (args.KeyboardDevice.IsKeyDown(System.Windows.Input.Key.LeftCtrl) && args.KeyboardDevice.IsKeyDown(System.Windows.Input.Key.Space))
                {
                    this.ShowCompletionWindow(GuessCompletionPrefix());

                    args.Handled = true;
                }
                else if (args.Key == System.Windows.Input.Key.RightShift || args.Key == System.Windows.Input.Key.LeftShift || args.Key == System.Windows.Input.Key.CapsLock || args.Key == System.Windows.Input.Key.Delete)
                {
                    return;
                }
                else if (completionWindow != null && completionWindow.Visible == true && (args.Key == System.Windows.Input.Key.Return || args.Key == System.Windows.Input.Key.Tab || args.Key == System.Windows.Input.Key.Space))
                {
                    this.AcceptCompletion();

                    args.Handled = true;
                }
                else if (completionWindow != null && completionWindow.Visible == true && args.Key == System.Windows.Input.Key.Back){
                    if(this.completionWindow.Str == ""){
                        this.CloseCompletionWindow();
                    }
                    else{
                        this.completionWindow.Str = this.completionWindow.Str.Substring(0, this.completionWindow.Str.Length - 1);
                    }
                }
                else if (completionWindow != null && completionWindow.Visible == true && args.Key == System.Windows.Input.Key.Down)
                {
                    this.completionWindow.SelectNextItem();

                    args.Handled = true;
                }
                else if (completionWindow != null && completionWindow.Visible == true && args.Key == System.Windows.Input.Key.Up)
                {
                    this.completionWindow.SelectPrevItem();

                    args.Handled = true;
                }
                else if(code.Length == 1 && completionWindow != null && completionWindow.Visible == true && (Char.IsLetterOrDigit(code[0]) || Char.IsLetterOrDigit('_'))){
                    this.completionWindow.Str += code[0];

                    if(this.completionWindow.state == CompletionWindow.State.Accepted){
                        args.Handled = true;
                    }
                }
                else //if (args.Key == System.Windows.Input.Key.Escape || args.Key == System.Windows.Input.Key.Left || args.Key == System.Windows.Input.Key.Right)
                {
                    this.CloseCompletionWindow();
                }
            };

            editor.PreviewMouseDown += delegate(object o, System.Windows.Input.MouseButtonEventArgs args)
            {
                this.CloseCompletionWindow();
            };

            editor.TextArea.LostFocus += delegate(object o, RoutedEventArgs args)
            {
                args.Handled = false;

                if (completionWindow == null || !completionWindow.Visible) return;

                this.WriteToConsole((this.completionWindow.Focused || this.editor.TextArea.IsFocused).ToString());

                if (!(this.completionWindow.Focused || this.editor.TextArea.IsFocused))
                {
                    this.CloseCompletionWindow();
                }
            };

        }

        void ShowCompletionWindow(string text){
            this.completionStartPosition = this.editor.CaretOffset - text.Length;
            
            if(this.completionWindow == null){
                this.completionWindow = new CompletionWindow();
            }

            this.completionWindow.state = CompletionWindow.State.Open;
            

            /*this.completionWindow = new ICSharpCode.AvalonEdit.CodeCompletion.CompletionWindow(editor.TextArea);
            this.completionWindow.Width = 300;
            IList<ICSharpCode.AvalonEdit.CodeCompletion.ICompletionData> data = this.completionWindow.CompletionList.CompletionData;

            foreach (ICSharpCode.AvalonEdit.CodeCompletion.ICompletionData item in this.completionData)
            {
                data.Add(item);
            }

            this.completionWindow.CloseAutomatically = false;
            this.completionWindow.Topmost = true;

            this.completionWindow.Show();

            this.completionWindow.Focus();

            this.completionWindow.CompletionList.SelectItemWithStart(text);

            this.editor.Focus();

            

            this.completionWindow.Closed += delegate
            {
                this.completionWindow = null;
            };*/

            //if(this.completionWindow.Vi) this.completionWindow.Show();

            this.Controls.Add(this.completionWindow);

            this.completionWindow.UpdatePosition();

            this.completionWindow.SelectFirstVisible();

            this.completionWindow.BringToFront();

            this.completionWindow.Visible = true;

            this.completionWindow.Str = text;
        }

        public void CloseCompletionWindow(){
            if(this.completionWindow != null) {
                this.completionWindow.Visible = false;

                this.completionWindow.state = CompletionWindow.State.Closed;
            }
        }

        public void AcceptCompletion(){
            editor.Text = 
                editor.Text.Substring(0, this.completionStartPosition) + 
                this.completionWindow.SelectedText +
                (this.completionWindow.selectedItem.original.GetType() == typeof(GGenMethod) ? "()" : "") +
                editor.Text.Substring(editor.CaretOffset);
            editor.Focus();

            editor.CaretOffset = this.completionStartPosition + this.completionWindow.SelectedText.Length;

            if (this.completionWindow.selectedItem.original.GetType() == typeof(GGenMethod)){
                editor.CaretOffset++;

                if(((GGenMethod) this.completionWindow.selectedItem.original).parameters.Count == 0){
                    editor.CaretOffset++;
                }
            }

            this.CloseCompletionWindow();

            this.completionWindow.state = CompletionWindow.State.Accepted;
        }

        string GuessCompletionPrefix(){
            if(this.editor.CaretOffset == 0 || this.editor.CaretOffset >= this.editor.Text.Length) return "";
            
            int i = this.editor.CaretOffset;


            char c = editor.Text[i - 1];
            while((Char.IsLetterOrDigit(c) || c == '_') && i > 0){
                i--;
                c = editor.Text[i];
            }

            if (i == this.editor.CaretOffset) return "";

            return editor.Text.Substring(i + 1, this.editor.CaretOffset - i - 1);
        }
    }
}