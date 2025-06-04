#include <cstdint>

namespace color_transform
{
uint32_t rgb_to_uint32(uint8_t r, uint8_t g, uint8_t b)
{
  return (r << 24) | (g << 16) | (b << 8) | 255;
}
}  // namespace color_transform