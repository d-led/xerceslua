MSBuild.exe %~dp0\..\xerces-c\projects\Win32\VC12\xerces-all\xerces-all.sln  /t:Build /p:Configuration=Release
MSBuild.exe %~dp0\..\xerces-c\projects\Win32\VC12\xerces-all\xerces-all.sln  /t:Build /p:Configuration=Debug
xcopy /Y %~dp0\..\xerces-c\Build\Win32\VC12\Release\xerces-c_3* %~dp0\..\..\bin
xcopy /Y %~dp0\..\xerces-c\Build\Win32\VC12\Debug\xerces-c_3* %~dp0\..\..\bin
