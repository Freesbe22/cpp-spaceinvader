#include <cstdint>

struct Buffer
{
  size_t width, height;
  uint32_t* data;
};

void buffer_clear(Buffer* buffer, uint32_t color)
{
  for (size_t i = 0; i < buffer->width * buffer->height; ++i)
  {
    buffer->data[i] = color;
  }
}