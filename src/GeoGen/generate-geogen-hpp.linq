<Query Kind="Program" />

public static IEnumerable<string> GetAllFiles(string path, Func<FileInfo, bool> checkFile = null)
{
   string mask = Path.GetFileName(path);
   if (string.IsNullOrEmpty(mask))
       mask = "*.*";
   path = Path.GetDirectoryName(path);
   string[] files = Directory.GetFiles(path, mask, SearchOption.AllDirectories);
   foreach (string file in files)
   {
       if (checkFile == null || checkFile(new FileInfo(file)))
           yield return file;
   }
}

void Main()
{
	Directory.SetCurrentDirectory (Path.GetDirectoryName (Util.CurrentQueryPath));

	string str = "// Header file used for including GeoGen into other projects" + Environment.NewLine + Environment.NewLine + "// Machine generated file, do not edit.";
  
	foreach(var file in GetAllFiles("./", p => p.Extension == ".hpp" && p.Name != "GeoGen.hpp" && p.Name != "AntlrRaiiWrappers.hpp"))
	{
		str += "#include \"" + file + "\"" + Environment.NewLine;
	}
	
	File.WriteAllText("GeoGen.hpp", str.Replace('\\', '/').Replace("./", "").Dump());
}