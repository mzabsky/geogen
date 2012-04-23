namespace GeoGen.Studio.PlugIns
{
    using System.ComponentModel;
    using System.Windows.Input;

    using GeoGen.Studio.PlugIns.Services;
    using GeoGen.Studio.Utilities;
    using GeoGen.Studio.Utilities.Collections;
    using GeoGen.Studio.Utilities.PlugInBase;

    /// <summary>
    /// Displays a window with overview of recently opened files and quick actions.
    /// </summary>
    public partial class WelcomeScreen : ControlBase, IQuickActionDisplay, INotifyPropertyChanged
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="WelcomeScreen"/> class.
        /// </summary>
        public WelcomeScreen()
        {
            this.QuickActions = new PriorityObservableCollection<QuickAction>();

            this.OpenFileCommand = new RelayCommand(p => this.FileService.OnOpened(this, (string)p));

            InitializeComponent();
        }

        /// <summary>
        /// Gets the file service which takes file manipulation requests from the recent files section.
        /// </summary>
        public IFileService FileService { get; private set; }

        /// <summary>
        /// Gets the open file command.
        /// </summary>
        public ICommand OpenFileCommand { get; private set; }

        /// <summary>
        /// Gets the collection of quick actions.
        /// </summary>
        public PriorityObservableCollection<QuickAction> QuickActions { get; private set; }        

        /// <summary>
        /// Registers this plug-in to the dock manager.
        /// </summary>
        /// <param name="dockManager">The dock manager.</param>
        public void Register(IDockManager dockManager)
        {
            dockManager.AddAsDocumentContent(this, "Welcome Screen", true);
        }

        /// <summary>
        /// Registers the file service.
        /// </summary>
        /// <param name="fileService">The file service.</param>
        public void Register(IFileService fileService)
        {
            this.FileService = fileService;
        }
        
        /// <inheritdoc/>
        public void RegisterQuickAction(QuickAction quickAction)
        {
            this.QuickActions.Add(quickAction);
        }
    }
}
