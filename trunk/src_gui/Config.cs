using System;

namespace GeoGen_Studio
{
    public class Config
    {
		public string templateFile;
        public bool openLastFileOnStartup;
        public string lastFile;
        public string scintillaDefinitonsFile;

		/* Execution settings */
        public string parameters;
        public bool goToOutputViewAfterExec;
        public string geoGenPath;
        public string geoGenWorkingDirectory;
        public string scriptTempFile;
        public string mainMapOutputFile;
		
		/* Output Viewer settings */
        public bool enableBitmapPreloading;
        public string overlayDirectory;
        public bool enableOverlayPreloading;

		/* Documentation Viewer settings */
        public string documentationPath;
		
		/* Window layout settings */
        public int mainSplitter;
        public bool sidebarOrientation;
        
        public Config()
	    {
            this.LoadDefaults();
	    }

        private void Serialize(string file, Config config)
        {
            // make sure the config directory exists
            if (!System.IO.Directory.Exists("../config"))
            {
                System.IO.Directory.CreateDirectory("../config");
            }

            System.Xml.Serialization.XmlSerializer xs = new System.Xml.Serialization.XmlSerializer(typeof(Config));

            System.IO.StreamWriter writer = System.IO.File.CreateText(file);
            
            xs.Serialize(writer, config);

            writer.Flush();
            writer.Close();
        }

        private static Config Deserialize(string file)
        {
            System.Xml.Serialization.XmlSerializer xs = new System.Xml.Serialization.XmlSerializer(System.Type.GetType("Config"));

            System.IO.StreamReader reader = System.IO.File.OpenText(file);

            Config c = (Config)xs.Deserialize(reader);

            reader.Close();

            return c;
        }

        private void LoadDefaults() 
        { 
            /* Basic settings */
            templateFile = "./../examples/template.nut";
            openLastFileOnStartup = false;
            lastFile = "../examples";
            scintillaDefinitonsFile = "../config/scintilla.xml";

            /* Execution settings */
            parameters = "2000 2000";
            goToOutputViewAfterExec = true;
            geoGenPath = "../bin/geogen.exe";
            geoGenWorkingDirectory = "../temp/studio_maps/temp";
            scriptTempFile = "../../studio_temp.nut";
            mainMapOutputFile = "../main.bmp";

            /* Output Viewer settings */
            overlayDirectory = "../overlays";
            enableBitmapPreloading = true;
            enableOverlayPreloading = true;

            /* Documentation Viewer settings */
            documentationPath = "../documentation.html";
        }

        public void Save()
        {
            Serialize("../config/studio.xml", this);
        }

        public static Config Load()
        {
            // try to load the config
            try
            {
                Config c = Deserialize("../config/studio.xml");

                return c;
            }
            // something went wrong -> create a new config file
            catch (System.Exception)
            {
                Config c = new Config();

                // save the newly created cofig
                c.Save();

                return c;
            }
        }

        public void LoadInterfaceSettings()
        {
            
        }

        public void SaveInterfaceSettings()
        {
            
        }
    }
}
