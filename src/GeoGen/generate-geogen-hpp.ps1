param($ProjectDir)

function GetFiles($path = $pwd, [string[]]$include, [string[]]$exclude) 
{ 
    foreach ($item in Get-ChildItem $path)
    {
        if (!($include | Where {$item -like $_}) -and !(Test-Path $item -PathType Container)) { continue }
		if ($exclude | Where {$item -like $_}) { continue }

        if (Test-Path $item -PathType Container) 
        {
			# $item
            foreach ($file in (GetFiles $item.FullName $include $exclude))
			{
				$file
			}
        } 
        else 
        { 		
            $f = Get-Item $item.FullName | Resolve-Path -Relative
			$f = "$f"
			$f = $f.Replace('.\', '')
			$f = $f.Replace('\', '/')
			$f
        }
    } 
}

$str = "#pragma once`n`n/// @file GeoGen.hpp Header file used for including GeoGen into other projects.`n// Machine generated file, do not edit.`n`n"
$files = Get-ChildItem -Path $ProjectDir -Filter *.hpp -Recurse # GetFiles $ProjectDir "*.hpp" "AntlrRaiiWrappers.hpp"

# $files

foreach ($file in $files)
{
	if($file -like "AntlrRaiiWrappers.hpp") { continue; }

	$file = Get-Item $file.FullName | Resolve-Path -Relative
	$file = $file.Replace('.\', '')
	$file = $file.Replace('\', '/')

	$str += "#include `"$file`"`n"
	$sourceFile = "$ProjectDir/$file"
	$destinationFile = "$ProjectDir/../../include/GeoGen/$file"
	# "Copying $sourceFile to $destinationFile"
	if(!(Test-Path -Path $destinationFile ))
	{
		New-Item -ItemType File -Path $destinationFile -Force
	}
	copy $sourceFile $destinationFile
}

$str | out-file -filepath "$ProjectDir/../../include/GeoGen/GeoGen.hpp"