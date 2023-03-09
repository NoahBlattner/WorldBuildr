/**
 * @file TagsManager.cpp
 * @brief Définition de la classe TagsManager.
 * @author Noah Blattner
 * @date Mars 2023
 */

#include <QMessageBox>

#include "TagsManager.h"

QList<QString>* TagsManager::m_pTags = new QList<QString>();

//! Ajoute un tag à la liste
//! \param tag Tag à ajouter
void TagsManager::addTag(const QString &tag) {
    if (m_pTags->contains(tag)) { // Si le tag existe déjà
        // Affichage d'un message d'erreur
        QMessageBox::warning(nullptr, "Warning", "Tag already exists");
        return;
    }

    m_pTags->append(tag);
}

//! Supprime un tag
//! \param tag Tag à supprimer
void TagsManager::removeTag(const QString &tag) {
    m_pTags->removeOne(tag);
}

//! Supprime tous les tags
void TagsManager::clearTags() {
    m_pTags->clear();
}

//! Retourne la liste des tags
//! \return Liste des tags
QList<QString> TagsManager::getTags() {
    // Retourner une copie de la liste
    return QList<QString>(*m_pTags);
}
