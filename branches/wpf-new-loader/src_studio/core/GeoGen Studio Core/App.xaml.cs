using System;
using System.Collections.Generic;
using System.Windows;
using System.Reflection;
using GeoGen.Studio.PlugInLoader;

namespace GeoGen.Studio
{
    /// <summary>
    /// Implementation of <see cref="Application"/> for this program.
    /// </summary>
    public partial class App
    {
        /// <summary>
        /// Name of the application.
        /// </summary>
        /// <value>The name.</value>
        public static string Name 
        { 
            get
            {
                return "GeoGen Studio";
            } 
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="App"/> class.
        /// </summary>
        /// <exception cref="T:System.InvalidOperationException">More than one instance of the <see cref="T:System.Windows.Application"/> class is created per <see cref="T:System.AppDomain"/>.</exception>
        public App()
        {
            this.InitializeComponent();

            try
            {
                // Load "plug-ins" from this executable.
                //OldLoader.ParseAssembly(Assembly.GetExecutingAssembly());

                // Load plug-ins from the plug-in directory.
                //OldLoader.ParseDirectory("./plugins");

                // Create plug-in instances and register their relationships.
                //OldLoader.ExecuteAllRegistrators();

                // Check if all plug-ins successfully loaded.
                List<string> messages = new List<string>();
                /*foreach (Registrator registrator in OldLoader.Registrators)
                {
                    if(!registrator.Failed) continue;

                    // don't show missing dependency errors for optional registrators
                    if(registrator.State == RegistratorState.UnimplementedInterface && !registrator.IsEnabled)
                    {
                        continue;
                    }

                    string message = registrator + ": " + registrator.State;

                    if(registrator.Exception != null){
                        message += " (" + registrator.Exception + ")";
                    }

                    messages.Add(message);
                }

                if(messages.Count > 0){
                    UI.MessageBox.Show("One or more plug-ins completely or partially failed to load" + Environment.NewLine + Environment.NewLine + "Details:" + Environment.NewLine + Environment.NewLine + String.Join(Environment.NewLine + Environment.NewLine, messages));
                }*/
            }
            // Any unhandled exception in this phase means the application cannot continue.
            catch(Exception e){
                UI.MessageBox.Show("GeoGen Studio plug-in loader failed with following message:" + Environment.NewLine + Environment.NewLine + e.Message + Environment.NewLine + Environment.NewLine + "The application will now be terminated.");
                Application.Current.Shutdown();
            }
        }
    }
}
