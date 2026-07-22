module;

#include <cmath>
#include <cstdint>
#include <expected>
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

  enum class CellState : std::uint8_t { 
    Free,
    Occupied,
    Unknown 
  };

  enum class GridError : std::uint8_t {
    None,
    InvalidDimensions,
    InvalidResolution,
    InvalidOrigin,
    CellCountOverflow,
    OutOfBounds
  };

  class OccupancyGrid final {
  public:
    using Cells = std::vector<CellState>;

    [[nodiscard]]
    static auto Create(std::uint32_t width, std::uint32_t height, double resolution, WorldPoint origin) -> std::expected<OccupancyGrid, GridError> {
 
      GridError error{GridError::None};
      const auto cellCount{static_cast<std::uint64_t>(width) * static_cast<std::uint64_t>(height)};

      if (width == 0 || height == 0) {
        error = GridError::InvalidDimensions;
      } else if (!std::isfinite(resolution) || resolution <= 0.0) {
        error = GridError::InvalidResolution;
      } else if (!std::isfinite(origin._x) || !std::isfinite(origin._y)) {
        error = GridError::InvalidOrigin;
      } else if (cellCount > std::vector<CellState>{}.max_size()) {
        error = GridError::CellCountOverflow;
      }
      return error == GridError::None ? std::expected<OccupancyGrid, GridError>
        {OccupancyGrid{ width, height, resolution, origin, static_cast<std::size_t>(cellCount)}} : std::unexpected{error};
    }

    [[nodiscard]]
    auto GetCellCentre(const GridCell &gridCell) const -> std::expected<WorldPoint, GridError> {
      /*
      origin = (-25.0, -10.0)
      resolution = 0.5 m
      width = 100
      */
      GridError error{GridError::None};
      return std::unexpected{error};
    }

    [[nodiscard]]
    auto GetCellState(const GridCell &gridCell) -> std::expected<CellState, GridError> {
      GridError error{GridError::None};
      std::size_t index{0};

      if (!Contains(gridCell)) {
        error = GridError::OutOfBounds;
      } else {
        index = StorageIndex(gridCell);
      }
      return error == GridError::None ? std::expected<CellState, GridError>{_cells[index]} : std::unexpected{error};
    }

    [[nodiscard]]
    auto SetCellState(const GridCell &gridCell, const CellState state) -> std::expected<void, GridError> {
      GridError error{GridError::None};
            
      if (!Contains(gridCell)) {
        error = GridError::OutOfBounds;
      } else {
        const auto index = StorageIndex(gridCell);
        _cells[index] = state;
      }
      return error == GridError::None ? std::expected<void, GridError>{} : std::unexpected{error};
    }

    [[nodiscard]]
    auto WorldToGrid(const WorldPoint &worldPoint) const -> std::expected<GridCell, GridError> { 
      GridError error{GridError::None};
      return std::unexpected{error};
    }

  private:
    OccupancyGrid(std::uint32_t width, std::uint32_t height, std::double_t resolution, WorldPoint origin, std::size_t cells)
      : _width(width), _height(height), _resolution(resolution), _origin(origin), _cells(cells, CellState::Unknown) {}

    [[nodiscard]]
    auto StorageIndex(const GridCell &cell) const -> std::size_t {
      return static_cast<std::size_t>(cell._y) * _width + static_cast<std::size_t>(cell._x);
    }

    [[nodiscard]]
    auto Contains(const GridCell& cell) const -> bool {
    return cell._x >= 0 && cell._y >= 0 && static_cast<std::uint32_t>(cell._x) < _width && static_cast<std::uint32_t>(cell._y) < _height;
    }

    std::uint32_t _width;
    std::uint32_t _height;
    std::double_t _resolution;
    WorldPoint _origin;
    Cells _cells;
  };
} // namespace motion