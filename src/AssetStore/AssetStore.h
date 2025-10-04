//
// Created by kuyba on 9/13/2025.
//

#ifndef ASSETSTORE_H
#define ASSETSTORE_H
#include <unordered_map>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class AssetStore {
private:
    std::unordered_map<std::string, SDL_Texture*> textures;
    std::unordered_map<std::string, TTF_Font*> fonts;
    //TODO Create store for sounds and fonts
public:
    AssetStore();

    ~AssetStore();
    void ClearAssets();
    void AddTexture (SDL_Renderer* renderer
        ,const std::string& assetId
        ,const std::string& filePath);

    SDL_Texture* GetTexture(const std::string& assetId);
    void AddFont(const std::string& assetId, const std::string& filePath, int fontSize);
    TTF_Font* GetFont(const std::string& assetId);
};

#endif //ASSETSTORE_H
