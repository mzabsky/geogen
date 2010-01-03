using System;
using System.ComponentModel;

namespace GeoGen_Studio
{
    [System.Xml.Serialization.XmlInclude(typeof(Main.ActionAfterExectution))]
    public class Config
    {

        public string templateFile;
        public bool openLastFileOnStartup;
        public string lastFile;
        public string scintillaDefinitonsFile;
        public string parameters;
        public Main.ActionAfterExectution actionAfterExec;
        public string geoGenPath;
        public string mainMapOutputFile;
        public string scriptTempFile;
        public string geoGenWorkingDirectory;
        public string overlayDirectory;
        public string documentationPath;
        public int heightScale;
        public bool wireframe;

		/* Window layout settings */
        public int mainSplitter;
        public int sidebarSplitter;
        public Main.SidebarMode sidebarMode;
        public bool showStatusbar;
        public bool showSidebar;
        public bool showToolbar;
        public bool showConsole;
        public bool showParameters;
        public bool wordWrap;
        public bool lineBreaks;
        public bool whitespace;
        public int editorZooom;
        

        [CategoryAttribute("Paths"), DescriptionAttribute("Path to the template file used when creating new file."), DefaultValue("./../examples/template.nut")]
        public string TemplateFile
        {
            get { return templateFile; }
            set { templateFile = value; }
        }

        [CategoryAttribute("Startup"), DescriptionAttribute("Should the last opened file be opened when starting the application?"), DefaultValue(false)]
        public bool OpenLastFileOnStartup
        {
            get { return openLastFileOnStartup; }
            set { openLastFileOnStartup = value; }
        }

        [CategoryAttribute("Paths"), DescriptionAttribute("Path to the XML file with syntax highlighting and autocompletion definitions."), DefaultValue("../config/scintilla.xml")]
        public string ScintillaDefinitonsFile
        {
            get { return scintillaDefinitonsFile; }
            set { scintillaDefinitonsFile = value; }
        }

        [CategoryAttribute("Script Execution"), DescriptionAttribute("Last used parameter string."), DefaultValue("")]
        public string Parameters
        {
            get { return parameters; }
            set { parameters = value; }
        }

        [CategoryAttribute("Script Execution"), DescriptionAttribute("What should the program do once script execution is finished?"), DefaultValue(Main.ActionAfterExectution.GoTo3DOutput)]
        public Main.ActionAfterExectution ActionAfterExec
        {
            get { return actionAfterExec; }
            set { actionAfterExec = value; }
        }

        [CategoryAttribute("Paths"), DescriptionAttribute("Path to a compatible GeoGen executable."), DefaultValue("../bin/geogen.exe")]
        public string GeoGenPath
        {
            get { return geoGenPath; }
            set { geoGenPath = value; }
        }

        [CategoryAttribute("Paths"), DescriptionAttribute("Base directory for all GeoGen temporary files."), DefaultValue("../temp/studio_maps/temp")]
        public string GeoGenWorkingDirectory
        {
            get { return geoGenWorkingDirectory; }
            set { geoGenWorkingDirectory = value; }
        }

        [CategoryAttribute("Paths"), DescriptionAttribute("Path where temporary script will be saved, relative to GeoGenWorkingDirectory."), DefaultValue("../../studio_temp.nut")]
        public string ScriptTempFile
        {
            get { return scriptTempFile; }
            set { scriptTempFile = value; }
        }

        [CategoryAttribute("Paths"), DescriptionAttribute("Path where main output map will be saved, relative to GeoGenWorkingDirectory."), DefaultValue("../main.bmp")]
        public string MainMapOutputFile
        {
            get { return mainMapOutputFile; }
            set { mainMapOutputFile = value; }
        }

        [CategoryAttribute("Paths"), DescriptionAttribute("Directory where overlays are stored."), DefaultValue("../overlays")]
        public string OverlayDirectory
        {
            get { return overlayDirectory; }
            set { overlayDirectory = value; }
        }

        [CategoryAttribute("Paths"), DescriptionAttribute("Path to main documentation file."), DefaultValue("../documentation.html")]
        public string DocumentationPath
        {
            get { return documentationPath; }
            set { documentationPath = value; }
        }
		
        
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
            System.Xml.Serialization.XmlSerializer xs = new System.Xml.Serialization.XmlSerializer(typeof(Config));

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
            lastFile = "";
            scintillaDefinitonsFile = "../config/scintilla.xml";

            /* Execution settings */
            parameters = "";
            actionAfterExec = Main.ActionAfterExectution.GoTo3DOutput;
            geoGenPath = "../bin/geogen.exe";
            geoGenWorkingDirectory = "../temp/studio_maps/temp";
            scriptTempFile = "../../studio_temp.nut";
            mainMapOutputFile = "../main.bmp";

            /* Output Viewer settings */
            overlayDirectory = "../overlays";

            /* Documentation Viewer settings */
            documentationPath = "../documentation.html";
        }

        public void Save()
        {
            Main.Get().SaveInterfaceSettings();
            Serialize("../config/studio.xml", this);
        }

        public static void Load()
        {
            Main main = Main.Get();
            
            // try to load the config
            try
            {
                Config c = Deserialize("../config/studio.xml");

                main.config = c;

                main.LoadInterfaceSettings();
            }
            // something went wrong -> create a new config file
            catch (System.Exception)
            {
                Config c = new Config();

                main.config = c;

                // save the newly created cofig
                c.Save();
            }
        }
    }
}
