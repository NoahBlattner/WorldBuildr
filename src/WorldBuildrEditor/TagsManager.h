/**
 * @file TagsManager.h
 * @brief Définition de la classe TagsManager.
 * @author Noah Blattner
 * @date Mars 2023
 */

#ifndef WORLDBUILDR_TAGSMANAGER_H
#define WORLDBUILDR_TAGSMANAGER_H

class QString;

#include <QList>

/**
 * @brief Gestionnaire de tags.
 *
 * Cette classe permet de gérer les tags.
 * Elle permet d'ajouter, de supprimer et de récupérer la liste des tags.
 *
 * Tout les tags sont stockés dans une liste statique.
 */
class TagsManager {

public:
    static void addTag(const QString& tag);
    static void removeTag(const QString& tag);
    static void clearTags();

    static QList<QString> getTags();

private:
    static QList<QString>* m_pTags;

};

#endif //WORLDBUILDR_TAGSMANAGER_H
