using GeoGen.Studio.Utilities;
using GeoGen.Studio.Utilities.Collections;
using System;

namespace GeoGen.Studio.PlugIns
{
    class GeneratorUI: GeoGen.Studio.Utilities.PlugInBase.Object
    {
        public void Register(IGenerator generator, IEditor editor, IMenuBar menuBar)
        {
            MenuEntry generatorMenu = new MenuEntry(
                header: "Generator",
                priority: -4,
                items: new PriorityObservableCollection()
                {
                    new MenuEntry(
                        header: "Execute",
                        priority: 10,
                        inputGestureText: "F5",
                        command: new RelayCommand(
                            param => generator.Start(editor.Text),
                            param => generator.IsReady
                        )
                    ),
                    new MenuEntry(
                        header: "Abort",
                        priority: 9,
                        inputGestureText: "F6",
                        command: new RelayCommand(
                            param => generator.Abort(),
                            param => !generator.IsReady
                        )
                    ),
                }
            );

            menuBar.AddMenu(generatorMenu);
        }
    }
}
