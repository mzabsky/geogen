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
using System.IO;

namespace GeoGen.Studio
{
    static class Program
    {

        public static string BasePath = "../";
        public static readonly string AlternateBasePath = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + "/GeoGen/";
 
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
#if DEBUG
#else
            // we will be catching "uncaught exception" and show them in some nice fashion
            Application.SetUnhandledExceptionMode(UnhandledExceptionMode.CatchException);
            Application.ThreadException += UncaughtExceptionHandler;
            AppDomain.CurrentDomain.UnhandledException += delegate(object o, UnhandledExceptionEventArgs a){
                Program.UncaughtExceptionHandler(null, new System.Threading.ThreadExceptionEventArgs(a.ExceptionObject as Exception));
            };
#endif
            Main main = null;

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            main = new Main();

            if(args.Length > 0){
                try{
                    if(System.IO.File.Exists(args[0])){
                        main.fileFromShell = args[0];
                    }
                }
                catch{}
            }

            

            // use local config if possible, use my documents config otherwise
            if (
                System.Reflection.Assembly.GetExecutingAssembly().Location.Contains(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86)) ||
                System.Reflection.Assembly.GetExecutingAssembly().Location.Contains(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles)) || 
                (!File.Exists(Program.BasePath + "config/studio.xml") && Directory.Exists(Program.AlternateBasePath)))
            {
                Program.BasePath = Program.AlternateBasePath;
            }


                // emergency save will be done by the UncaughtExceptionHandler in release mode
#if DEBUG 
                System.IO.File.WriteAllText("../emergencyBackup.nut", main.editorBackup);    
#endif

                Application.Run(main);
        }

        static void UncaughtExceptionHandler(object sender, System.Threading.ThreadExceptionEventArgs args){
            // don't allow any further exceptions out
            try
            {                

                // first of all, make sure the edited script is not lost
                System.IO.File.WriteAllText(Program.BasePath + "/emergencyBackup.nut", GeoGen.Studio.Main.Get().editorBackup);

                // also write the exception to disk
                System.IO.File.WriteAllText(Program.BasePath + "/exceptionInfo.txt", args.Exception.ToString());

                // hide the loading dialog (if it is displayed)
                try
                {
                    GeoGen.Studio.Main.Get().loading.Hide();
                }
                catch { }


                Crash dialog = new Crash();

                dialog.errorInfo.Text = args.Exception.ToString();

                dialog.ShowDialog();
            }
            catch { }
            finally{
                /* Kill the process the the program doesn't freeze after user clicks OK in the exception dialog
                 * (the exception handled by the AppDomain.CurrentDomain.UnhandledException handler doesn't appear
                 * as caught). */
                System.Diagnostics.Process.GetCurrentProcess().Kill();
            }
        }
    }
}
