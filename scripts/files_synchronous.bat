@echo off

xcopy /E /Y "..\..\library\engieering\Graphics\ref\*" ".\ref\"
xcopy /E /Y "..\..\library\engieering\Graphics\notes\*" ".\docs\"

git add *