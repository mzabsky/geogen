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
    public partial class App : Application
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="App"/> class.
        /// </summary>
        /// <exception cref="T:System.InvalidOperationException">More than one instance of the <see cref="T:System.Windows.Application"/> class is created per <see cref="T:System.AppDomain"/>.</exception>
        public App()
        {
            try
            {
                // Load "plug-ins" from this executable.
                Loader.ParseAssembly(Assembly.GetExecutingAssembly());

                // Load plug-ins from the plug-in directory.
                Loader.ParseDirectory("./plugins");

                // Create plug-in instances and register their relationships.
                Loader.ExecuteAllRegistrators();

                // Check if all plug-ins successfully loaded.
                List<string> messages = new List<string>();
                foreach (Registrator registrator in Loader.Registrators)
                {
                    if(!registrator.Failed) continue;

                    if(registrator.FailureType == RegistratorFailureType.UnimplementedInterface && !registrator.IsRequired)
                    {
                        continue;
                    }

                    string message = registrator.ToString() + ": " + registrator.FailureType.ToString();

                    if(registrator.Exception != null){
                        message += " (" + registrator.Exception + ")";
                    }

                    messages.Add(message);
                }

                if(messages.Count > 0){
                    MessageBox.Show("One or more plug-ins completely or partially failed to load" + Environment.NewLine + Environment.NewLine + "Details:" + Environment.NewLine + Environment.NewLine + String.Join(Environment.NewLine, messages), "GeoGen Studio", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
            // Any unhandled exception in this phase means the application cannot continue.
            catch(Exception e){
                MessageBox.Show("GeoGen Studio plug-in loader failed with following message:" + Environment.NewLine + Environment.NewLine + e.Message, "GeoGen Studio", MessageBoxButton.OK, MessageBoxImage.Error);
                Application.Current.Shutdown();
            }
        }
    }
}
