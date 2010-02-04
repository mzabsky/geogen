using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace GeoGen_Studio
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Main main = null;

            // we will be catching "uncaught exception" and show them in some nice fashion
#if DEBUG                 
#else
            try
            {
#endif
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);

                main = new Main();

                Application.Run(main);


#if DEBUG 
                System.IO.File.WriteAllText("../emergencyBackup.nut", main.editorBackup);    
#else
            }
            catch (Exception e)
            {
                // don't allow any further exceptions out
                try
                {
                    // first of all, make sure the edited script is not lost
                    System.IO.File.WriteAllText("../emergencyBackup.nut", main.editorBackup);

                    // also write the exception to disk
                    System.IO.File.WriteAllText("../exceptionInfo.txt", e.ToString());

                    // show the exception info dialog only in release mode

                    Crash dialog = new Crash();

                    dialog.errorInfo.Text = e.ToString();

                    dialog.ShowDialog();

                }
                catch { };
            }
#endif
        }
    }
}
