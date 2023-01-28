:: Ce script génère la documentation HTML à partir de la doc intégrée au code.
:: Pour qu'il fonctionne, il nécessite que l'utilitaire Doxygen soit installé sur le poste.
:: Auteur : Jérôme Conus
:: Date   : 06.09.2022

@ECHO OFF

::Avant de changer de dossier, on mémorise le dossier actuel
set OLDDIR=%CD%

:: Pour que doxygen fonctionne, il faut que le dossier de travail soit celui qui contient le Doxyfile
cd ..\src
"C:\Program Files\doxygen\bin\doxygen" Doxyfile

:: Rétablissement du dossier d'origine
cd %OLDDIR%
