param([string]$filePath)

# USER DEFINED
$sse = "D:\Games\SteamLibrary\steamapps\common\Skyrim Special Edition"
$outDir = "D:\Games\SteamLibrary\steamapps\common\Skyrim Special Edition\Data"


# GRAB DIRECTORIES
$sseSrc = $sse + "\Data\Scripts\Source"
$usrSrc = (Get-Location)
$papyrus = $sse + "\Papyrus Compiler\PapyrusCompiler.exe"
$flags = $sseSrc + "\TESV_Papyrus_Flags.flg"


# COMPILE PAPYRUS SCRIPTS
& "$papyrus" "$filePath" -f="$flags" -i="$sseSrc;$usrSrc\Scripts\Source" -o="$outDir\Scripts"
