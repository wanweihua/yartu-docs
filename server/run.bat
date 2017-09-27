ECHO OFF

reg Query "HKLM\Hardware\Description\System\CentralProcessor\0" | find /i "x86" > NUL && set OS=32||set OS=64

ECHO.
ECHO ----------------------------------------
ECHO copy file to converter
ECHO ----------------------------------------

mkdir "%~dp0\App_Data"
mkdir "%~dp0\FileConverter\bin"
mkdir "%~dp0\FileConverter\bin\HtmlFileInternal"

cd /D "%~dp0\FileConverter\bin" || goto ERROR
copy "..\..\..\core\build\bin\win_64\icudt.dll" "."
copy "..\..\..\core\build\bin\icu\win_%OS%\icudt55.dll" "."
copy "..\..\..\core\build\bin\icu\win_%OS%\icuuc55.dll" "."
copy "..\..\..\core\build\lib\DoctRenderer.config" "."
copy "..\..\..\core\build\lib\win_%OS%\doctrenderer.dll" "."
copy "..\..\..\core\build\lib\win_%OS%\HtmlRenderer.dll" "."
copy "..\..\..\core\build\lib\win_%OS%\DjVuFile.dll" "."
copy "..\..\..\core\build\lib\win_%OS%\XpsFile.dll" "."
copy "..\..\..\core\build\lib\win_%OS%\PdfReader.dll" "."
copy "..\..\..\core\build\lib\win_%OS%\PdfWriter.dll" "."
copy "..\..\..\core\build\lib\win_%OS%\HtmlFile.dll" "."
copy "..\..\..\core\build\lib\win_%OS%\UnicodeConverter.dll" "."
copy "..\..\..\core\build\lib\win_%OS%\HtmlFileInternal.exe" ".\HtmlFileInternal"
xcopy /s/h/e/k/c/y/q "..\..\..\core\build\cef\win_%OS%" ".\HtmlFileInternal"
copy "..\..\..\core\build\bin\win_%OS%\x2t.exe" "."

"..\..\..\core\build\bin\AllFontsGen\win_%OS%.exe" "%windir%\Fonts" "%~dp0\..\sdkjs\common\AllFonts.js" "%~dp0\..\sdkjs\common\Images" "%~dp0\FileConverter\bin\font_selection.bin"

mkdir "%~dp0\SpellChecker\dictionaries"
cd /D "%~dp0\SpellChecker" || goto ERROR
xcopy /s/e/k/c/y/q "..\..\dictionaries" ".\dictionaries"

ECHO.
ECHO ----------------------------------------
ECHO Start build skd-all.js
ECHO ----------------------------------------
CD /D %~dp0\..\sdkjs\build
call npm install -g grunt-cli
call npm install
call grunt --src="./configs" --level=WHITESPACE_ONLY --formatting=PRETTY_PRINT


ECHO.
ECHO ----------------------------------------
ECHO Install node.js modules 
ECHO ----------------------------------------

CD /D %~dp0\DocService || goto ERROR
call npm install

cd /D ..\Common || goto ERROR
call npm install

cd /D ..\FileConverter || goto ERROR
call npm install

cd /D ..\SpellChecker || goto ERROR
call npm install

SET RUN_DIR=%~dp0
SET NODE_ENV=development-windows
SET NODE_CONFIG_DIR=%RUN_DIR%\Common\config

cd "%RUN_DIR%\DocService\sources"
start /min /b node server.js
start /min /b node gc.js

cd "%RUN_DIR%\FileConverter\sources"
start /min /b node convertermaster.js

cd "%RUN_DIR%\SpellChecker\sources"
start /min /b node server.js

:ERROR
:SUCCESS
pause
