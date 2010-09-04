using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Windows;
using System.Reflection;

namespace GeoGen_Studio
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public App()
        {
            try
            {
                PlugInLoader loader = new PlugInLoader();

                /* Load "plug-ins" from this executable */
                loader.ParseAssembly(Assembly.GetExecutingAssembly());

                /* Load plug-ins from the plug-in directory */
                loader.ParseDirectory("./plugins");

                /* Create plug-in instances and register their relationships */
                loader.ExecuteAllRegistrators();

                /* Check if all plug-ins successfully loaded */
                List<string> messages = new List<string>();
                foreach (Registrator registrator in loader.Registrators)
                {
                    if(!registrator.Failed) continue;

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
            /* Any unhandled exception in this phase means the application cannot continue */
            catch(Exception e){
                MessageBox.Show("GeoGen Studio plug-in loader failed with following message:" + Environment.NewLine + Environment.NewLine + e.Message, "GeoGen Studio", MessageBoxButton.OK, MessageBoxImage.Error);
                Application.Current.Shutdown();
            }
        }
    }
}
