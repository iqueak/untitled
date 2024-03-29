//
// Created by bohdan.yurov on 13.02.2023.
//

#include "Block.h"
#include "../Utils/Logger.h"
#include "map"

using namespace std;

namespace Block{

    /*** registered block types with block data (texture coords for face etc.)  ***/
    // TODO попробовать все таки использовать привычный std::map вместо обычного массива
    // TODO убрать статик
    //static BlockType  BlockTypes[int(BlockIDs::_COUNT)];
    std::map<int, BlockType> BlockTypes;

    //std::vector<BlockType> BlockTypes  = std::vector<BlockType>();

    /*** register block type with one texture index ***/
    void Block::blockRegistry(BlockIDs id, BlockType & type, int textureIndex) {
        for (auto &i : type.coordForFace) {
            TextureManager::GetAtlas()->getTextureCoords(textureIndex, i);
        }
        BlockTypes[int(id)] = type;
        //Logger::Log("BLOCK",string(string("Block ") + type.name + string(": is registered!")));
    }

    /*** register block type with multiple texture indexes ***/
    void Block::blockRegistry(BlockIDs id, BlockType & type, int textureIndexes[6]) {
        for (int i = 0; i < 6; i++) {
            TextureManager::GetAtlas()->getTextureCoords(textureIndexes[i], type.coordForFace[i]);
        }
        BlockTypes[int(id)] = type;
        //Logger::Log("BLOCK",string(string("Block ") + type.name + string(": is registered!")));
    }

    BlockType & Block::getBlockData(uint8_t id) {
        return BlockTypes[int(id)];
    }

    void Init() {
        BlockType dr = BlockType{"Dirt",false};
        Block::blockRegistry(BlockIDs::DIRT,dr,2);

        BlockType bd = BlockType{"Bedrock",false};
        Block::blockRegistry(BlockIDs::BEDROCK,bd,17);

        BlockType st = BlockType{"Stone",false};
        Block::blockRegistry(BlockIDs::STONE,st,1);

        BlockType KEK = BlockType{"Grass",false};
        int txind[6] = {3,3,3,3,2,0}; //TODO возможно стоит использовать std::vector
        Block::blockRegistry(BlockIDs::GRASS,KEK,txind);
    }

}

