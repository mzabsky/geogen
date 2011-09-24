using System;
using System.IO;
using System.Windows.Input;
using GeoGen.Studio.PlugInLoader;
using GeoGen.Studio.UI;
using GeoGen.Studio.Utilities;
using GeoGen.Studio.Utilities.Collections;
using GeoGen.Studio.Utilities.IO;

namespace GeoGen.Studio.PlugIns
{
    /// <summary>
    /// 
    /// </summary>
    public sealed partial class WelcomeScreen: IQuickActionDisplay
    {
        public ICommand OpenFileCommand { get; private set; }
        public PriorityObservableCollection<QuickAction> QuickActions { get; private set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="WelcomeScreen"/> class.
        /// </summary>
        public WelcomeScreen()
        {
            this.QuickActions = new PriorityObservableCollection<QuickAction>();

            this.OpenFileCommand = new RelayCommand(p => FileService.OnFileOpened(this, (FileInfo) p));

            InitializeComponent();
        }

        /// <summary>
        /// Registers tis plug-in to the dock manager.
        /// </summary>
        /// <param name="dockManager">The dock manager.</param>
        public void Register(IDockManager dockManager)
        {
            dockManager.AddAsDocumentContent(this, "Welcome Screen", true);
        }

        public void RegisterQuickAction(QuickAction quickAction)
        {
            this.QuickActions.Add(quickAction);
        }
    }
}
