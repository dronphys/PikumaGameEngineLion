//
// Created by kuyba on 9/13/2025.
//

#ifndef ASSETSTORE_H
#define ASSETSTORE_H
#include <unordered_map>
#include <string>
#include <SDL.h>


class AssetStore {
private:
    std::unordered_map<std::string, SDL_Texture*> textures;
    //TODO Create store for sounds and fonts
public:
    AssetStore();

    ~AssetStore();
    void ClearAssets();
    void AddTexture (SDL_Renderer* renderer
        ,const std::string& assetId
        ,const std::string& filePath);

    SDL_Texture* GetTexture(const std::string& assetId);
};



#endif //ASSETSTORE_H
