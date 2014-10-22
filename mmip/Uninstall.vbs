SDB.Tools.OutputDebugString("Entering Uninstall.vbs...")

Dim fso: set fso = CreateObject("Scripting.FileSystemObject")

' delete all the presets stored in the plugin folder
ExtractTo = fso.BuildPath(SDB.CurrentAddonInstallRoot, "Plugins\Milkdrop2\")

'If the extraction location does not exist create it.
If fso.FolderExists(ExtractTo) Then
   fso.GetFolder(ExtractTo).Delete
End If

SDB.Tools.OutputDebugString("Exiting Uninstall.vbs...")
