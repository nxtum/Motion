module;

#include <cmath>
#include <cstdint>
#include <expected>
#include <vector>
#include <limits>

export module motion.grid;
export namespace motion {

  struct GridCell {
    std::int32_t x;
    std::int32_t y;
  };

  struct WorldPoint {
    double x;
    double y;
  };

  enum class CellState : std::uint8_t { 
    Free,
    Occupied,
    Unknown 
  };

  enum class GridError : std::uint8_t {
    InvalidDimensions,
    InvalidResolution,
    InvalidOrigin,
    InvalidExtent,
    CellCountOverflow,
    OutOfBounds,
    InvalidWorldPoint
  };

  /*
  TODO:
  CellCountOverflow should prolly be changed to a different name, not technically an overflow, more so storage
  Add geometry access, like get height etc...
  Start to implement tests
  std::bad_alloc? think how to solve this
  */

  class OccupancyGrid final {
  public:
    [[nodiscard]]
    static auto Create(std::uint32_t width, std::uint32_t height, double resolution, WorldPoint origin) -> std::expected<OccupancyGrid, GridError> {
      constexpr auto maximumCoordinate{static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max())};

      if (width == 0 || height == 0 || width > maximumCoordinate || height > maximumCoordinate) {
        return std::unexpected{GridError::InvalidDimensions};
      }

      if (!std::isfinite(resolution) || resolution <= 0.0) {
        return std::unexpected{GridError::InvalidResolution};
      }

      if (!std::isfinite(origin.x) || !std::isfinite(origin.y)) {
        return std::unexpected{GridError::InvalidOrigin};
      }

      const auto maxX{origin.x + static_cast<double>(width) * resolution};
      const auto maxY{origin.y + static_cast<double>(height) * resolution};
      if (!std::isfinite(maxX) || !std::isfinite(maxY) || maxX <= origin.x || maxY <= origin.y) {
        return std::unexpected{GridError::InvalidExtent};
      }

      const auto cellCount{static_cast<std::uint64_t>(width) * static_cast<std::uint64_t>(height)};
      if (cellCount > Cells{}.max_size()) {
        return std::unexpected{GridError::CellCountOverflow};
      }
      return OccupancyGrid{width, height, resolution, origin, static_cast<std::size_t>(cellCount)};
    }

    [[nodiscard]]
    auto CellCentre(GridCell cell) const -> std::expected<WorldPoint, GridError> {
      if (!Contains(cell)) {
        return std::unexpected{GridError::OutOfBounds};
      }

      return WorldPoint{
        origin_.x + (static_cast<double>(cell.x) + 0.5) * resolution_,
        origin_.y + (static_cast<double>(cell.y) + 0.5) * resolution_
      };
    }

    [[nodiscard]]
    auto StateAt(GridCell cell) const -> std::expected<CellState, GridError> {
      if (!Contains(cell)) {
        return std::unexpected{GridError::OutOfBounds};
      }
      return cells_[StorageIndex(cell)];
    }

    [[nodiscard]]
    auto SetState(GridCell cell, CellState state) -> std::expected<void, GridError> {
      if (!Contains(cell)) {
        return std::unexpected{GridError::OutOfBounds};
      }
      cells_[StorageIndex(cell)] = state;
      return {};
    }

    [[nodiscard]]
    auto WorldToCell(WorldPoint point) const -> std::expected<GridCell, GridError> {
      const auto gridX{(point.x - origin_.x) / resolution_};
      const auto gridY{(point.y - origin_.y) / resolution_};

      if (!std::isfinite(gridX) || !std::isfinite(gridY)) {
        return std::unexpected{GridError::InvalidWorldPoint};
      }

      if (gridX < 0.0 || gridY < 0.0 || gridX >= static_cast<double>(width_) || gridY >= static_cast<double>(height_)) {
        return std::unexpected{GridError::OutOfBounds};
      }
      return GridCell{static_cast<std::int32_t>(std::floor(gridX)), static_cast<std::int32_t>(std::floor(gridY))};
    }

    [[nodiscard]]
    auto Contains(GridCell cell) const noexcept -> bool {
      return cell.x >= 0 && cell.y >= 0 && static_cast<std::uint32_t>(cell.x) < width_ && static_cast<std::uint32_t>(cell.y) < height_;
    }

  private:
    using Cells = std::vector<CellState>;

    OccupancyGrid(std::uint32_t width, std::uint32_t height, double resolution, WorldPoint origin, std::size_t cellCount)
      : width_(width), height_(height), resolution_(resolution), origin_(origin), cells_(cellCount, CellState::Unknown) {}

    // Precondition: Contains(cell)
    [[nodiscard]]
    auto StorageIndex(GridCell cell) const noexcept -> std::size_t {
      return static_cast<std::size_t>(cell.y) * static_cast<std::size_t>(width_) + static_cast<std::size_t>(cell.x);
    }

    std::uint32_t width_;
    std::uint32_t height_;
    double resolution_;
    WorldPoint origin_;
    Cells cells_;
  };
} // namespace motion