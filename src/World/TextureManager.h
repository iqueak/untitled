//
// Created by bohdan.yurov on 13.02.2023.
//
#pragma once
#ifndef UNTITLED_TEXTUREMANAGER_H
#define UNTITLED_TEXTUREMANAGER_H

#endif //UNTITLED_TEXTUREMANAGER_H

#include "raylib.h"
#include "string"
#include "../Utils/Logger.h"

namespace TextureManager {

    class TextureMap{

    public:
        Texture2D texture2D{};

        int rowsCount;
        int columnsCount;
        float textureSize;

        TextureMap(const char *path, int rows, int cols){
            texture2D = LoadTexture(path);

            rowsCount = rows;
            columnsCount = cols;
            textureSize = 1.0f / float(columnsCount);
        }

        void getTextureCoords(int index,Vector2 (&coords)[4]) const;

    };

    TextureMap* GetAtlas();

    void Init();
}