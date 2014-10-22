SDB.Tools.OutputDebugString("Entering Install.vbs...")

Dim fso: set fso = CreateObject("Scripting.FileSystemObject")

' unzip the presets and textures
' http://stackoverflow.com/a/911796/2448947

' The location of the zip file.
ZipFile=fso.BuildPath(SDB.CurrentAddonInstallRoot, "Plugins\Milkdrop2\favorite_presets_2013_12.zip")
' The folder the contents should be extracted to.
ExtractTo=fso.BuildPath(SDB.CurrentAddonInstallRoot, "Plugins\Milkdrop2\")

' If the extraction location does not exist create it.
If NOT fso.FolderExists(ExtractTo) Then
   fso.CreateFolder(ExtractTo)
End If


SDB.Tools.OutputDebugString("CurrentAddonInstallRoot: " & SDB.CurrentAddonInstallRoot)
SDB.Tools.OutputDebugString("ZipFile " & ZipFile)
SDB.Tools.OutputDebugString("ExtractTo: " & ExtractTo)

' Extract the presets zip
set objShell = CreateObject("Shell.Application")
set FilesInZip=objShell.NameSpace(ZipFile).items
objShell.NameSpace(ExtractTo).CopyHere(FilesInZip)

' Extract the data zip
ZipFile=fso.BuildPath(SDB.CurrentAddonInstallRoot, "Plugins\Milkdrop2\data.zip")
ExtractTo=fso.BuildPath(SDB.CurrentAddonInstallRoot, "Plugins\")

set FilesInZip=objShell.NameSpace(ZipFile).items
objShell.NameSpace(ExtractTo).CopyHere(FilesInZip)

Set fso = Nothing
Set objShell = Nothing

SDB.Tools.OutputDebugString("Exiting Install.vbs...")
