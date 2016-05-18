#ifndef Sprite_h
#define Sprite_h

//Defined classes
class Sprite {
  private:
  int x_coor;
  int y_coor;
  char character_sprite;
  bool visible;
  public:
  int getX();
  int getY();
  void setX(int x);
  void setY(int y);
  void setCharacterSprite(char sprite);
  char getCharacterSprite();
  bool isVisible();
  void setVisible(bool isVisible);
};

#endif