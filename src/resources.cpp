/**
  \file
  \brief    Méthodes utilitaires permettant d'accéder aux ressources.
  \author   JCO
  \date     février 2014
  \version  1.0
*/
#include "resources.h"

#include <QCoreApplication>
#include <QDir>
#include <QDebug>

namespace GameFramework {
    static QString resourcesLocation;
/**
Cette fonction retourne le chemin absolu du répertoire res.

Lors du premier appel à cette fonction, l'emplacement des ressources est
recherché en partant du répertoire de travail de l'application et en remontant
la hiérarchie des répertoires autant que nécessaire.
Cette technique permet de tenir compte du fait que l'emplacement des ressources
peut varier selon que l'application est démarrée depuis l'environnement de
développement, ou depuis une installation spécifique, ou selon l'OS sur lequel
elle tourne.

Lors d'une installation spécifique, les ressources se trouvent normalement au
même niveau que l'application :

\verbatim
+--ce_projet/
   |
   +--res/
   +ce_projet.exe
\endverbatim

Lors du développement, sous Windows, les ressources se trouvent à un emplacement
plus élévé dans la hiérarchie :

\verbatim
+--ce_projet/
   |
   +--build-ce_projet.../
   |  |
   |  +--debug/
   |  |  |
   |  |  +ce_projet.exe
   |  |
   |  +--release/
   |     |
   |     +ce_projet.exe
   |
   +--doc/
   +--res/
   +--src/
\endverbatim

\return une chaîne de caractères contenant le chemin absolu du répertoire res.
*/
    QString resourcesPath() {
        // Si l'emplacement des ressources n'a pas encore été trouvé :
        if (resourcesLocation.isEmpty()) {
            QDir resourceDir = QDir(qApp->applicationDirPath());
            while (!resourceDir.exists("res") && resourceDir.cdUp());

            if (resourceDir.exists("res")) {
                resourceDir.cd("res");
                resourcesLocation = resourceDir.absolutePath() + QDir::separator();
                qInfo() << "Resources dir found : " + resourcesLocation;
            } else
                qCritical() << "Resources dir not found !";
        }
        return resourcesLocation;
    }

    //! Indique le chemin d'accès aux resources images.
    //! \return une chaîne de caractères contenant le chemin absolu du répertoire des images.
    QString imagesPath() {
        return resourcesPath() + QString("images") + QDir::separator();
    }
}
