using System;
using System.ComponentModel;

namespace GeoGen_Studio
{
    [System.Xml.Serialization.XmlInclude(typeof(Main.ActionAfterExectution))]
    [System.Xml.Serialization.XmlInclude(typeof(ViewportManager.ModelDetailLevel))]
    [System.Xml.Serialization.XmlInclude(typeof(ViewportManager.ViewportBackground))]
    [System.Xml.Serialization.XmlInclude(typeof(OpenTK.Vector4))]
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
        public string lastImportedFile;
        public string lastExportedFile;
        public bool exportRescaleMode;
        public string lastImportedTexture;
        public string lastExportedOutput;
        public string lastExportedScreenshot;

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

        private ViewportManager.ModelDetailLevel modelDetailLevel;
        public bool wireframe;
        public OpenTK.Vector4 ambientLightColor;
        public OpenTK.Vector4 directionalLightColor;
        public float lightAzimuth;
        public float lightElevation;
        public bool lightEnabled;
        public bool enableTerrainUnderZero;
        public bool enable3d;
        public int defaultTextureOverlay;
        public ViewportManager.ViewportBackground backgroundColor3d;

        

        [CategoryAttribute("Paths"), DescriptionAttribute("Path to the template file used when creating new file."), DefaultValue("./../examples/template.nut")]
        public string TemplateFile
        {
            get { return templateFile; }
            set { templateFile = value; }
        }

        [CategoryAttribute("Startup"), DescriptionAttribute("Should the last opened file be opened when starting the application?"), DefaultValue(true)]
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

        [CategoryAttribute("Paths"), DescriptionAttribute("Path where main output map will be saved, relative to GeoGenWorkingDirectory."), DefaultValue("../main.shd")]
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

        [CategoryAttribute("3D View"), DescriptionAttribute("Highest allowed amount of polygons for the terrain model in the 3D view. Higher settings require more powerful graphics card and the models take longer to build. Has major impact on performance."), DefaultValue(ViewportManager.ModelDetailLevel.Medium_512x512Polygons)]
        public ViewportManager.ModelDetailLevel ModelDetailLevel
        {
            get { return modelDetailLevel; }
            set { modelDetailLevel = value; }
        }

        [CategoryAttribute("3D View"), DescriptionAttribute("Should the underwater terrain be modelled in 3D? Change of this setting will take effect once the model is rebuilt."), DefaultValue(false)]
        public bool EnableTerrainUnderZero
        {
            get { return enableTerrainUnderZero; }
            set { enableTerrainUnderZero = value; }
        }

        [CategoryAttribute("3D View"), DescriptionAttribute("Enable/disable the 3D view."), DefaultValue(true)]
        public bool Enable3D
        {
            get { return enable3d; }
            set { enable3d = value; }
        }

        [CategoryAttribute("3D View"), DescriptionAttribute("Color covering the unpainted areas of the 3D viewport."), DefaultValue(ViewportManager.ViewportBackground.Black)]
        public ViewportManager.ViewportBackground BackgroundColor3d
        {
            get { return backgroundColor3d; }
            set { backgroundColor3d = value; }
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

            try
            {
                Config c = (Config)xs.Deserialize(reader);
                return c;
            }
            // make sure the config file is closed so it can be recreated in case of failure
            finally
            {
                reader.Close();
            }
        }

        private void LoadDefaults() 
        { 
            /* Basic settings */
            templateFile = "./../examples/template.nut";
            openLastFileOnStartup = true;
            lastFile = "";
            scintillaDefinitonsFile = "../config/scintilla.xml";
            lastImportedFile = "";
            lastExportedFile = "";
            lastImportedTexture = "";
            lastExportedScreenshot = "";
            lastExportedOutput = "";
            
            exportRescaleMode = false;

            /* Execution settings */
            parameters = "";
            actionAfterExec = Main.ActionAfterExectution.GoTo3DOutput;
            geoGenPath = "../bin/geogen.exe";
            geoGenWorkingDirectory = "../temp/studio_maps/temp";
            scriptTempFile = "../../studio_temp.nut";
            mainMapOutputFile = "../main.shd";

            /* Output Viewer settings */
            overlayDirectory = "../overlays";

            /* Documentation Viewer settings */
            documentationPath = "../documentation.html";

            /* 3D view */
            modelDetailLevel = ViewportManager.ModelDetailLevel.Medium_512x512Polygons;
            enableTerrainUnderZero = false;
            enable3d = true;
            defaultTextureOverlay = 7;

            /* 3D light */
            ambientLightColor = new OpenTK.Vector4(0.2f, 0.2f, 0.2f, 1.0f);
            OpenTK.Vector4 directionalLightColor = new OpenTK.Vector4(0.6f, 0.6f, 0.6f, 1.0f);
            lightAzimuth = 0.0f;
            lightElevation = 0.5f;
            lightEnabled = true;
            backgroundColor3d = ViewportManager.ViewportBackground.Black;
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
                main.WriteToConsole("Could not open config file, falling back to defaults.");

                Config c = new Config();

                main.config = c;

                // save the newly created cofig
                c.Serialize("../config/studio.xml", c);
            }
        }
    }
}
