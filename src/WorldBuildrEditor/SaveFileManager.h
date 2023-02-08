/**
 * @file SaveFileManager.h
 * @brief Définition de la classe SaveFileManager.
 * @author Noah Blattner
 * @date Février 2023
 */

#ifndef WORLDBUILDR_SAVEFILEMANAGER_H
#define WORLDBUILDR_SAVEFILEMANAGER_H

class QString;
class EditorManager;
class QJsonObject;

class SaveFileManager {
public:
    static void save(EditorManager *editorManager, QString path);
    static void load(EditorManager *editorManager, QString path);

private:
    static QJsonObject convertEditorToJsonObject(EditorManager *editorManager);
    static QJsonArray convertSpritesToJsonObject(const QList<EditorSprite*>& sprites);
};


#endif //WORLDBUILDR_SAVEFILEMANAGER_H
