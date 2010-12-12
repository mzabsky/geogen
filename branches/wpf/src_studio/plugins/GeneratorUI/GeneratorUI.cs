using System;
using System.Windows.Input;
using GeoGen.Studio.Utilities;

namespace GeoGen.Studio.PlugIns
{
    class GeneratorUI: GeoGen.Studio.Utilities.PlugInBase.Object
    {
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
    }
}
