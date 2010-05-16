/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    GeoGen is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GeoGen.  If not, see <http://www.gnu.org/licenses/>.

*/

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

                // show the exception info dialog only in release mode
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

                    // hide the loading dialog (if it is displayed)
                    try{
                        main.loading.Hide();
                    }
                    catch {}


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
