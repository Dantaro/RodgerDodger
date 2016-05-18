#include "Sprite.h"

int Sprite::getX() { return x_coor;}
int Sprite::getY() { return y_coor;}
void Sprite::setX(int x) {x_coor = x;}
void Sprite::setY(int y) {y_coor = y;}
void Sprite::setCharacterSprite(char sprite) {character_sprite = sprite;}
char Sprite::getCharacterSprite() {return character_sprite;}
bool Sprite::isVisible() { return visible; }
void Sprite::setVisible(bool isVisible) { visible = isVisible; }
