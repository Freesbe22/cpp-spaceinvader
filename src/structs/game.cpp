#include <cstdint>
struct Player
{
  size_t x, y;
  size_t life;
};
struct Alien
{
  size_t x, y;
  uint8_t type;
};
struct Game
{
  size_t width, height;
  size_t num_aliens;
  Alien* aliens;
  Player player;
};