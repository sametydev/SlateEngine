#include "ShaderCache.h"

std::unordered_map<std::string, std::shared_ptr<DXVertexShader>> ShaderCache::vertexShaders;
std::unordered_map<std::string, std::shared_ptr<DXPixelShader>> ShaderCache::pixelShaders;

