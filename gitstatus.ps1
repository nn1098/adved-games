Param (
 [String]$Namespace,
 [String]$Project,
 [String]$GitRoot,
 [String]$HeaderFile="version.h"
)

Push-Location -LiteralPath $GitRoot

$VerFileHead = "`#pragma once`n`#include <string>`n`nnamespace $Namespace {`n"
$VerFileTail = "}`n"

$env:Path = $env:Path + ";C:\Program Files\Git\bin"

if ((Get-Command "git.exe" -ErrorAction SilentlyContinue) -eq $null) 
{ 
	$VerUrl  =  ' const std::string FullHash="0";'| Out-String
	$VersUrl  ='const std::string Hash="0";'| Out-String
	$VerDate = ' const std::string CommitDate="0";'| Out-String
}else{

	$VerUrl  = (git log -n 1 --format=format:"  const std::string FullHash=`\`"%H`\`";%n") | Out-String
	$VersUrl  = (git log -n 1 --format=format:"  const std::string Hash=`\`"%h`\`";%n") | Out-String
	$VerDate = (git log -n 1 --format=format:"  const std::string CommitDate=`\`"%ai`\`";%n") | Out-String
}

"Written $Project\" + (
  New-Item -Force -Path "$Project" -Name "$HeaderFile" -ItemType "file" -Value "$VerFileHead$VerUrl$VersUrl$VerDate$VerFileTail"
).Name + " as:"
""
Get-Content "$Project\$HeaderFile"
""

Pop-Location