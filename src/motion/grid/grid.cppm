module;
#include <cmath>
#include <cstdint>
#include <vector>
export module motion.grid;
export namespace motion {

/*
Grid coordinates:
- signed ints, origins at bottom left

World coorindates:
- floating point, measured in metres, cartesian system, origin defined by the
grid

GridCell:
- identifies a discrete cell
- does not mean centre or corner

Grid to world:
- returns the centre of the cell by default

World to grid:
- uses floor
- fails when outside grid
- never silently clamps

*/

struct GridCell {
  std::int32_t _x;
  std::int32_t _y;
};

struct WorldPoint {
  std::double_t _x;
  std::double_t _y;
};

enum class CellState : std::uint8_t { Free, Occupied, Unknown };

class OccupancyGrid final {
public:
  OccupancyGrid() = delete;
  OccupancyGrid(std::uint32_t width, std::uint32_t height,
                std::double_t resolution, WorldPoint origin);

  // TODO: Implement conversions
  auto GridToWorld();
  auto WorldToGrid();

  // TODO: Operations to Read/Write from CellState

private:
  std::uint32_t _width;
  std::uint32_t _height;
  std::double_t _resolution;
  WorldPoint _origin;
  std::vector<CellState> _cells;
};
} // namespace motion