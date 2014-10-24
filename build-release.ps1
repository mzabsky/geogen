function ZipFiles( $zipfilename, $sourcedir )
{
   Add-Type -Assembly System.IO.Compression.FileSystem
   $compressionLevel = [System.IO.Compression.CompressionLevel]::Optimal
   [System.IO.Compression.ZipFile]::CreateFromDirectory($sourcedir, $zipfilename, $compressionLevel, $false)
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

Start-Sleep -s 1
ZipFiles "$(Get-Location)/geogen.zip" "$(Get-Location)/release"