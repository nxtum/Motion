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

  enum class GridCreateError : std::uint8_t {
    InvalidDimensions,
    InvalidResolution,
    InvalidOrigin,
    InvalidExtent,
    GridTooLarge
  };

  enum class CoordinateError : std::uint8_t {
    OutOfBounds,
    InvalidWorldPoint
  };

  class OccupancyGrid final {
  public:
    [[nodiscard]] //std::bad_alloc
    static auto Create(std::uint32_t width, std::uint32_t height, double resolution, WorldPoint origin) -> std::expected<OccupancyGrid, GridCreateError> {
      constexpr auto maximumCoordinate{static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max())};

      if (width == 0 || height == 0 || width > maximumCoordinate || height > maximumCoordinate) {
        return std::unexpected{GridCreateError::InvalidDimensions};
      }

      if (!std::isfinite(resolution) || resolution <= 0.0) {
        return std::unexpected{GridCreateError::InvalidResolution};
      }

      if (!std::isfinite(origin.x) || !std::isfinite(origin.y)) {
        return std::unexpected{GridCreateError::InvalidOrigin};
      }

      const auto maxX{origin.x + static_cast<double>(width) * resolution};
      const auto maxY{origin.y + static_cast<double>(height) * resolution};
      if (!std::isfinite(maxX) || !std::isfinite(maxY) || maxX <= origin.x || maxY <= origin.y) {
        return std::unexpected{GridCreateError::InvalidExtent};
      }

      const auto cellCount{static_cast<std::uint64_t>(width) * static_cast<std::uint64_t>(height)};
      if (cellCount > Cells{}.max_size()) {
        return std::unexpected{GridCreateError::GridTooLarge};
      }
      return OccupancyGrid{width, height, resolution, origin, static_cast<std::size_t>(cellCount)};
    }

    [[nodiscard]]
    constexpr auto Width() const noexcept -> std::uint32_t {return width_;}

    [[nodiscard]]
    constexpr auto Height() const noexcept -> std::uint32_t {return height_;}

    [[nodiscard]]
    constexpr auto Resolution() const noexcept -> double {return resolution_;}

    [[nodiscard]]
    constexpr auto Origin() const noexcept -> WorldPoint {return origin_;}

    [[nodiscard]]
    auto CellCentre(GridCell cell) const -> std::expected<WorldPoint, CoordinateError> {
      if (!Contains(cell)) {
        return std::unexpected{CoordinateError::OutOfBounds};
      }
      return WorldPoint{
        origin_.x + (static_cast<double>(cell.x) + 0.5) * resolution_,
        origin_.y + (static_cast<double>(cell.y) + 0.5) * resolution_
      };
    }

    [[nodiscard]]
    auto StateAt(GridCell cell) const -> std::expected<CellState, CoordinateError> {
      if (!Contains(cell)) {
        return std::unexpected{CoordinateError::OutOfBounds};
      }
      return cells_[StorageIndex(cell)];
    }

    [[nodiscard]]
    auto SetState(GridCell cell, CellState state) -> std::expected<void, CoordinateError> {
      if (!Contains(cell)) {
        return std::unexpected{CoordinateError::OutOfBounds};
      }
      cells_[StorageIndex(cell)] = state;
      return {};
    }

    [[nodiscard]]
    auto WorldToCell(WorldPoint point) const -> std::expected<GridCell, CoordinateError> {
      const auto gridX{(point.x - origin_.x) / resolution_};
      const auto gridY{(point.y - origin_.y) / resolution_};

      if (!std::isfinite(gridX) || !std::isfinite(gridY)) {
        return std::unexpected{CoordinateError::InvalidWorldPoint};
      }

      if (gridX < 0.0 || gridY < 0.0 || gridX >= static_cast<double>(width_) || gridY >= static_cast<double>(height_)) {
        return std::unexpected{CoordinateError::OutOfBounds};
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