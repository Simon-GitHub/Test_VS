@echo off
setlocal enabledelayedexpansion  

for /r . %%a in (Debug) do (  
	if exist %%a (
		echo "delete" %%a
		rd /s /q "%%a" 
	)
)

for /r . %%a in (Release) do (  
	if exist %%a (
		echo "delete" %%a
		rd /s /q "%%a" 
	)
)

for /r . %%a in (bin) do (  
	if exist %%a (
		echo "delete" %%a
		rd /s /q "%%a" 
	)
)

for /r . %%a in (lib) do (  
	if exist %%a (
		echo "delete" %%a
		rd /s /q "%%a" 
	)
)

for /r . %%a in (x64) do (  
	if exist %%a (
		echo "delete" %%a
		rd /s /q "%%a" 
	)
)

for /r . %%a in (Win32) do (  
	if exist %%a (
		echo "delete" %%a
		rd /s /q "%%a" 
	)
)

REM 模糊删除文件夹*.dir
for /f "delims=" %%a in ('dir /ad/s/b "*.dir*"') do (
	if exist %%a (
		echo "delete" %%a
		rd /s /q "%%a" 
	)
)

for /r . %%a in (ipch) do (  
	if exist %%a (
		echo "delete" %%a
		rd /s /q "%%a" 
	)
)

for /r . %%a in (CMakeFiles) do (  
	if exist %%a (
		echo "delete" %%a
		rd /s /q "%%a" 
	)
)

REM 下面的for循环删除不了隐藏文件，因此隐藏的*.suo文件删除不了
for /r . %%a in (*.suo) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)

for /r . %%a in (*.sdf) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)

for /r . %%a in (*.vcxproj) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)

for /r . %%a in (*.vcxproj.user) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)

for /r . %%a in (*.vcxproj.filters) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)

for /r . %%a in (cmake_install.cmake) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)

for /r . %%a in (cmake_uninstall.cmake) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)

for /r . %%a in (*.sln) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)

for /r . %%a in (*.suo) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)

for /r . %%a in (install_manifest.txt) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)

for /r . %%a in (CMakeCache.txt) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)

for /r . %%a in (CPackConfig.cmake) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)

for /r . %%a in (CPackSourceConfig.cmake) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)

for /r . %%a in (ui_*.h) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)

for /r . %%a in (qrc_*.cpp) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)

for /r . %%a in (*.qrc.depends) do (  
	if exist %%a (
		echo "delete" %%a
		del "%%a" 
	)
)

pause