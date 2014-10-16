<Query Kind="Statements" />

var cd = (Path.GetDirectoryName (Util.CurrentQueryPath) + "\\..\\..\\bin\\").Dump();
Directory.SetCurrentDirectory (cd);

var path = "../bin/";
var prefix = "tile_";
var suffix = "_main.png";

var files = new DirectoryInfo(path).EnumerateFiles("*.png");
var regex = new Regex("^" + prefix + "(?<x>-?[0-9]+)_(?<y>-?[0-9]+)" + suffix);

var matches = files.Select(p => regex.Match(p.Name)).Where(p => p.Captures.Count > 0);

var tiles = matches.ToDictionary(
	p => Tuple.Create(
		int.Parse(p.Groups["x"].ToString()),
		int.Parse(p.Groups["y"].ToString())
	),
	p => p.ToString()
).Dump();

var xs = tiles.Select(p => p.Key.Item1).Distinct().OrderBy(p => p);
var ys = tiles.Select(p => p.Key.Item2).Distinct().OrderBy(p => p);

StringBuilder builder = new StringBuilder();
builder.AppendLine(@"
	<html>
		<header>
			<script src=""http://code.jquery.com/jquery-latest.min.js"" type=""text/javascript""></script>
			<script type=""text/javascript"">
				$( document ).ready(function() {
					$('table').click(function () {
						$('table').toggleClass('red');
					});
				});
			</script>
			<style>
			table.red td{outline: 1px solid red; outline-offset: -1px;}
			</style>
		</header>
		<body>
			<table cellspacing=""0"" cellpadding=""0"">");
foreach(var y in ys)
{
	builder.AppendLine("<tr>");
	foreach(var x in xs)
	{
		builder.AppendLine("<td><img src=\"" + path + prefix + x + "_" + y + suffix +"\" /></td>");
	}
	builder.AppendLine("</tr>");
}
builder.AppendLine("</table></body></html>");

File.WriteAllText("out.html", builder.ToString());
Process.Start("out.html");