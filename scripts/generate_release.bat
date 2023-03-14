:: Ce script g�n�re l'executable de gameframework en mode release pour le d�ploiement.
:: Auteur : J�r�me Conus
:: Date   : 06.1209.2022

@ECHO OFF

::Avant de changer de dossier, on m�morise le dossier actuel
::set OLDDIR=%CD%

set PATH=%PATH%;C:\Qt\Tools\mingw1120_64\bin

C:\Qt\6.4.0\mingw_64\bin\qmake.exe ..\src\WorldBuildr.pro

C:\Qt\Tools\mingw1120_64\bin\mingw32-make.exe -f Makefile qmake_all

:: Compilation et link
C:\Qt\Tools\mingw1120_64\bin\mingw32-make.exe -j2

:: Nettoyage
C:\Qt\Tools\mingw1120_64\bin\mingw32-make.exe clean -j2

:: Cr�ation du dossier release qui contiendra les fichiers de d�ploiement
mkdir ..\release
copy .\release\WorldBuildr.exe ..\release\ /Y
xcopy ..\res ..\release\res\ /E /Y

c:\Qt\6.4.0\mingw_64\bin\windeployqt.exe ..\release\WorldBuildr.exe

:: Suppression du dossier inutiles cr��s par windeployqt
rmdir ..\release\translations /S /Q
rmdir ..\release\tls /S /Q
:: R�tablissement du dossier d'origine
::cd %OLDDIR%

