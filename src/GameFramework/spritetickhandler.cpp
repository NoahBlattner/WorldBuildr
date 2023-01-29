/**
  \file
  \brief    Définition de la classe SpriteTickHandler.
  \author   JCO
  \date     octobre 2022
*/
#include "spritetickhandler.h"

#include "sprite.h"

//! Construit un gestionnaire de tick pour le Sprite donné.
//! \param pParentSprite  Sprite qui sera géré par ce gestionnaire.
SpriteTickHandler::SpriteTickHandler(Sprite* pParentSprite) {
    setParentSprite(pParentSprite);
}

//! Destructeur
SpriteTickHandler::~SpriteTickHandler() {

}

//! Indique quel sprite sera géré par ce gestionnaire.
void SpriteTickHandler::setParentSprite(Sprite* pParentSprite) {
    m_pParentSprite = pParentSprite;
}

//! \return un pointeur sur la scène à laquelle appartient le sprite géré
//! par ce gestionnaire.
GameScene* SpriteTickHandler::parentScene() const {
    return m_pParentSprite->parentScene();
}
