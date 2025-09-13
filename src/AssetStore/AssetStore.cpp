//
// Created by kuyba on 9/13/2025.
//

#include "AssetStore.h"
#include "../Logger/Logger.h"
#include <SDL_image.h>

AssetStore::AssetStore() {
    Logger::Log("Asset store constructor");
}

AssetStore::~AssetStore() {
    ClearAssets();
    Logger::Log("Asset store destructor");
}

void AssetStore::ClearAssets() {
    for (auto [assetId, texture]: textures) {
        SDL_DestroyTexture(texture);
    }
    textures.clear();
}

void AssetStore::AddTexture(SDL_Renderer* renderer ,const std::string &assetId, const std::string &filePath) {
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    textures.emplace(assetId, texture);
    Logger::Log("Texture with Id: " + assetId + " added to asset store");
}

SDL_Texture* AssetStore::GetTexture(const std::string &assetId) {
    return textures[assetId];
}
