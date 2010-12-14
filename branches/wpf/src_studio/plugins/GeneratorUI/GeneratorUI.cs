using System;
using System.Windows.Input;
using GeoGen.Studio.Utilities;
using GeoGen.Studio.Utilities.Context;

namespace GeoGen.Studio.PlugIns
{
    class GeneratorUI: GeoGen.Studio.Utilities.PlugInBase.Object
    {
        protected Context executingContext = new Context("Executing");

        /// <summary>
        /// Subscribes to generator events.
        /// </summary>
        /// <param name="generator">The generator.</param>
        public void Register(IGenerator generator)
        {
            generator.Started += delegate(object o, GenerationStartedEventArgs args)
            {
                if (!args.HeaderOnly)
                {
                    ContextManager.EnterContext(executingContext);
                }
            };

            generator.Aborted += delegate(object o, EventArgs args)
            {
                ContextManager.LeaveContext(executingContext);
            };

            generator.Failed += delegate(object o, GenerationFailedEventArgs args)
            {
                ContextManager.LeaveContext(executingContext);
            };

            generator.Finished += delegate(object o, GenerationFinishedEventArgs args)
            {
                ContextManager.LeaveContext(executingContext);
            };
        }

        public void Register(IGenerator generator, IEditor editor, IMenuBar menuBar, IMainWindow mainWindow)
        {
            ICommand executeCommand = new RelayCommand(
                param => generator.Start(editor.Text),
                param => generator.IsReady
            );

            ICommand abortCommand = new RelayCommand(
                param => generator.Abort(),
                param => !generator.IsReady
            );

            // Register hotkeys
            mainWindow.RegisterInputGesture(new KeyGesture(Key.F5), executeCommand);
            mainWindow.RegisterInputGesture(new KeyGesture(Key.F6), abortCommand);

            // Register window menu entries
            MenuEntry generatorMenu = new MenuEntry(
                header: "Generator",
                priority: -4,
                items: new MenuEntryObservableCollection()
                {
                    new MenuEntry(
                        header: "Execute",
                        priority: 10,
                        inputGestureText: "F5",
                        command: executeCommand
                    ),
                    new MenuEntry(
                        header: "Abort",
                        priority: 9,
                        inputGestureText: "F6",
                        command: abortCommand
                    ),
                }
            );

            menuBar.AddMenu(generatorMenu);
        }

        public void Register(IGenerator generator, IApplicationStatusDisplay applicationStatusDisplay)
        {
            applicationStatusDisplay.RegisterApplicationStatusContext(executingContext);
        }
    }
}
