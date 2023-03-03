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
    static void save(EditorManager *editorManager, QString savePath);
    static void load(EditorManager *editorManager, QString saveFilePath);
    static void import(EditorManager *editorManager, QString saveFilePath);

private:
    static const QString DEFAULT_SAVE_DIR;

    static QJsonDocument loadJsonDocument(QString saveFilePath);

    static QJsonObject convertEditorToJsonObject(EditorManager* editorManager);
    static QJsonArray convertTagsToJsonArray(const QList<QString>& tags);
    static QJsonArray convertSpritesToJsonArray(const QList<EditorSprite*>& sprites);

    static void loadJsonIntoEditor(EditorManager* editorManager, QJsonObject jsonObject);
    static void importJsonIntoEditor(EditorManager* editorManager, QJsonObject jsonObject);
    static void loadTagsFromJson(const QJsonArray& jsonArray);
    static QList<EditorSprite*> loadSpritesFromJson(const QJsonArray& jsonArray);
};


#endif //WORLDBUILDR_SAVEFILEMANAGER_H
