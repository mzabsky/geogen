function ZipFiles( $zipfilename, $sourcedir )
{
   Add-Type -Assembly System.IO.Compression.FileSystem
   $compressionLevel = [System.IO.Compression.CompressionLevel]::Optimal
   [System.IO.Compression.ZipFile]::CreateFromDirectory($sourcedir, $zipfilename, $compressionLevel, $false)
}

function ReplaceDoxygenMarkers($filename)
{
	$script = [IO.File]::ReadAllText($filename)
	$script = [Text.RegularExpressions.Regex]::Replace($script, "^/// \[[A-Za-z]+\]\s*", "`n", [Text.RegularExpressions.RegexOptions]::Multiline)
	$script = $script.Replace("`n`n`n", "`n`n")
	[IO.File]::WriteAllText($filename, $script)
}

del release -recurse 
del geogen.zip
mkdir release
mkdir release/geogen

copy bin/geogen.exe release/geogen
copy bin/zlib1.dll release/geogen
copy examples release/geogen -recurse
mkdir release/geogen/reference
copy documentation/ScriptingLanguage/* release/geogen/reference -recurse
copy COPYING release/geogen
copy README-RELEASE release/geogen
ren release/geogen/README-RELEASE README

#Add-Type -Assembly System.Text
Get-ChildItem "$(Get-Location)/release/geogen/examples" -Filter *.ggs | `
Foreach-Object{
    ReplaceDoxygenMarkers $_.FullName~
}

Start-Sleep -s 1
ZipFiles "$(Get-Location)/geogen.zip" "$(Get-Location)/release"