<Query Kind="Program" />

static IEnumerable<string> GetFiles(string path) {
    Queue<string> queue = new Queue<string>();
    queue.Enqueue(path);
    while (queue.Count > 0) {
        path = queue.Dequeue();
        try {
            foreach (string subDir in Directory.GetDirectories(path)) {
                queue.Enqueue(subDir);
            }
        }
        catch(Exception ex) {
            Console.Error.WriteLine(ex);
        }
        string[] files = null;
        try {
            files = Directory.GetFiles(path);
        }
        catch (Exception ex) {
            Console.Error.WriteLine(ex);
        }
        if (files != null) {
            for(int i = 0 ; i < files.Length ; i++) {
                yield return files[i];
            }
        }
    }
}

void Main()
{
	Directory.SetCurrentDirectory (Path.GetDirectoryName (Util.CurrentQueryPath));

	var paths = new [] {
		"../GeoGen",
		"../Console",
		"../GeoGen_Tests",
		"../ScriptingDocumentation",
		"../examples"
	};

	var files = paths.SelectMany(p => GetFiles(p));
	var exceptions = new [] {"MTRand.cpp", "MTRand.hpp", "generated"};
	
	var gplHeader = @"/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

";
	
	foreach(var file in files)
	{
	
		var fileInfo = new FileInfo(file);
		if(
			fileInfo.Extension != ".cpp" && 
			fileInfo.Extension != ".hpp" && 
			fileInfo.Extension != ".c" && 
			fileInfo.Extension != ".h" && 
			fileInfo.Extension != ".g")
		{
			continue;	
		}
		Console.WriteLine(file);
		
		bool ignore = false;
		foreach(var exception in exceptions)
		{
			if(file.IndexOf(exception) != -1)
			{
				ignore = true;
			}
		}
		
		if(ignore)
		{
			continue;
		}
		
		var content = File.ReadAllText(file);
		
		if(content.Length <= gplHeader.Length || content.Substring(0, gplHeader.Length) != gplHeader)
		{
			content = gplHeader + content;
			File.WriteAllText(file, content);
		}
	}
}