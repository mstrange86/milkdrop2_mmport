echo.
echo.
echo COPYING COMPILED FILES TO MEDIAMONKEY 

xcopy /e /y  %~dp0\vis_milk2\output\Winamp\plugins\*   c:\bin\MediaMonkey\Plugins\
xcopy    /y  %~dp0\vis_milk2\Release\vis_milk2.dll     c:\bin\MediaMonkey\Plugins\


