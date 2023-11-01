#include <SlateEngine/Engine/Graphics/Buffer/BufferCache.h>
std::map<void*, std::shared_ptr<DXVertexBuffer>> BufferCache::mVertexBuffers;
std::map<void*, std::shared_ptr<DXIndexBuffer>> BufferCache::mIndexBuffers;