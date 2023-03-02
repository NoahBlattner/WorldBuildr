//
// Created by blatnoa on 02.03.2023.
//

#include "TagsManager.h"

QList<QString>* TagsManager::m_pTags = new QList<QString>();

//! Ajoute un tag à la liste
//! \param tag Tag à ajouter
void TagsManager::addTag(const QString &tag) {
    m_pTags->append(tag);
}

//! Supprime un tag
//! \param tag Tag à supprimer
void TagsManager::removeTag(const QString &tag) {
    m_pTags->removeOne(tag);
}

//! Retourne la liste des tags
//! \return Liste des tags
QList<QString> TagsManager::getTags() {
    // Retourner une copie de la liste
    return QList<QString>(*m_pTags);
}
