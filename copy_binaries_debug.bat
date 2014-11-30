echo.
echo.
echo COPYING COMPILED FILES TO MEDIAMONKEY 

xcopy /e /y %~dp0\vis_milk2\output\Winamp\plugins\*     %appdata%\MediaMonkey\Plugins\
xcopy    /y %~dp0\vis_milk2\Debug\vis_milk2.dll         %appdata%\MediaMonkey\Plugins\


