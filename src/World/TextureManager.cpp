//
// Created by bohdan.yurov on 13.02.2023.
//

#include "iostream"
#include "TextureManager.h"


namespace TextureManager {

    TextureMap *TextureAtlas;
    static const char *path = "./../res/tex/spritesheet.png";

    void Init() {
        TextureManager::TextureAtlas = new TextureMap(path, 16, 16);
    }

    TextureMap *TextureManager::GetAtlas() {
        return TextureAtlas;
    }

    /*** Generate texture coords for BlockType from indexes a texture from texture atlas ***/
    void TextureMap::getTextureCoords(int index, Vector2 (&coords)[4]) const {
        //  Почему то текстуры имеют совсем другие координаты
        //  2---------1          0 x----> 1
        //  |         |          y
        //  |         |          |
        //  |         |          V
        //  3---------0          1

        float x = (index % rowsCount) / float(rowsCount);
        float y = (float(index) / float(columnsCount) - x) / float(columnsCount);

        coords[2].x = x;
        coords[2].y = y;

        coords[3].x = x;
        coords[3].y = y + textureSize;

        coords[0].x = x + textureSize;
        coords[0].y = y + textureSize;

        coords[1].x = x + textureSize;
        coords[1].y = y;
    }
}