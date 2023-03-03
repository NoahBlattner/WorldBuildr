//
// Created by blatnoa on 02.03.2023.
//

#ifndef WORLDBUILDR_TAGSMANAGER_H
#define WORLDBUILDR_TAGSMANAGER_H

class QString;

#include <QList>

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
