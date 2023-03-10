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

/**
 * @brief Gestionnaire de sauvegarde.
 *
 * Cette classe permet de gérer les sauvegardes de niveau.
 *
 * Elle permet de sauvegarder, de charger et d'importer des niveaux.
 *
 * save() permet de sauvegarder un niveau dans un fichier.
 * load() permet de charger un niveau à partir d'un fichier. (Les sprites générés remplacent les sprites actuels)
 * import() permet d'importer un niveau à partir d'un fichier. (Les sprites générés sont ajoutés aux sprites actuels)
 * Ces 3 fonctions nécessitent un EditorManager et un chemin de sauvegarde.
 * Si le chemin de sauvegarde est vide, le fichier sera sauvegardé dans le dossier par défaut.
 */
class SaveFileManager {
public:

    static const QString DEFAULT_SAVE_DIR;

    static void save(EditorManager *editorManager, QString savePath);
    static void load(EditorManager *editorManager, QString saveFilePath);
    static void import(EditorManager *editorManager, QString saveFilePath);

private:
    static QJsonDocument loadJsonDocument(QString saveFilePath);

    static QJsonObject convertEditorToJsonObject(EditorManager* editorManager);
    static QJsonArray convertTagsToJsonArray(const QList<QString>& tags);
    static QJsonArray convertSpritesToJsonArray(const QList<EditorSprite*>& sprites);

    static void loadJsonIntoEditor(EditorManager* editorManager, QJsonObject jsonObject);
    static void loadTagsFromJson(const QJsonArray& jsonArray);
    static QList<EditorSprite*> loadSpritesFromJson(const QJsonArray& jsonArray);
};


#endif //WORLDBUILDR_SAVEFILEMANAGER_H
