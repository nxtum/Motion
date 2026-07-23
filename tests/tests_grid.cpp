#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_test_macros.hpp>

#include <limits>
#include <cstdint>
import motion;

constexpr auto aboveMaximumCoordinate = static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max()) + 1U;
// TODO: Make valid width, height, resol, origin a constexpr at the top and replace tests

TEST_CASE("OccupancyGrid creation succeeds for valid inputs","[grid][create]") {
    const auto result = motion::OccupancyGrid::Create(
        10U,
        20U,
        0.5,
        motion::WorldPoint{1.0, 2.0}
    );

    REQUIRE(result.has_value());
    const auto& grid = result.value();
    CHECK(grid.Width() == 10U);
    CHECK(grid.Height() == 20U);
    CHECK_THAT(grid.Resolution(), Catch::Matchers::WithinAbs(0.5, 1e-9));
    CHECK_THAT(grid.Origin().x, Catch::Matchers::WithinAbs(1.0, 1e-9));
    CHECK_THAT(grid.Origin().y, Catch::Matchers::WithinAbs(2.0, 1e-9));
}

TEST_CASE("OccupancyGrid creation fails for invalid dimensions","[grid][create]") {
    SECTION("Width is zero") {
        const auto result = motion::OccupancyGrid::Create(
        0U,
        20U,
        0.5,
        motion::WorldPoint{1.0, 2.0}
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error() == motion::GridError::InvalidDimensions);
    }
    SECTION("Height is zero") {
        const auto result = motion::OccupancyGrid::Create(
        2U,
        0U,
        0.5,
        motion::WorldPoint{1.0, 2.0}
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error() == motion::GridError::InvalidDimensions);
    }
    SECTION("Width is above int32_t limit") {
        const auto result = motion::OccupancyGrid::Create(
        aboveMaximumCoordinate,
        10U,
        0.5,
        motion::WorldPoint{1.0, 2.0}
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error() == motion::GridError::InvalidDimensions);
    }
    SECTION("Height is above int32_t limit") {
        const auto result = motion::OccupancyGrid::Create(
        33U,
        aboveMaximumCoordinate,
        0.5,
        motion::WorldPoint{1.0, 2.0}
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error() == motion::GridError::InvalidDimensions);
    }
}

TEST_CASE("OccupancyGrid creation fails for invalid resolution","[grid][create]") {
    SECTION("Resolution is negative") {
        const auto result = motion::OccupancyGrid::Create(
        10U,
        20U,
        -3.5,
        motion::WorldPoint{1.0, 2.0}
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error() == motion::GridError::InvalidResolution);
    }
    SECTION("Resolution is zero") {
        const auto result = motion::OccupancyGrid::Create(
        10U,
        20U,
        0.0,
        motion::WorldPoint{1.0, 2.0}
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error() == motion::GridError::InvalidResolution);
    }
    SECTION("Resolution is infinity") {
        const auto result = motion::OccupancyGrid::Create(
        10U,
        20U,
        std::numeric_limits<double>::infinity(),
        motion::WorldPoint{1.0, 2.0}
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error() == motion::GridError::InvalidResolution);
    }
    SECTION("Resolution is negative infinity") {
        const auto result = motion::OccupancyGrid::Create(
        10U,
        20U,
        -std::numeric_limits<double>::infinity(),
        motion::WorldPoint{1.0, 2.0}
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error() == motion::GridError::InvalidResolution);
    }
    SECTION("Resolution is NaN") {
        const auto result = motion::OccupancyGrid::Create(
        10U,
        20U,
        std::numeric_limits<double>::quiet_NaN(),
        motion::WorldPoint{1.0, 2.0}
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error() == motion::GridError::InvalidResolution);
    }
}

TEST_CASE("OccupancyGrid creation fails for invalid origin","[grid][create]") {
    SECTION("Origin X is infinity") {
        const auto result = motion::OccupancyGrid::Create(
            10U,
            20U,
            0.5,
            motion::WorldPoint{std::numeric_limits<double>::infinity(), 5.0}
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error() == motion::GridError::InvalidOrigin);
    }

    SECTION("Origin X is negative infinity") {
        const auto result = motion::OccupancyGrid::Create(
            10U,
            20U,
            0.5,
            motion::WorldPoint{-std::numeric_limits<double>::infinity(), 5.0}
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error() == motion::GridError::InvalidOrigin);
    }

    SECTION("Origin X is NaN") {
        const auto result = motion::OccupancyGrid::Create(
            10U,
            20U,
            0.5,
            motion::WorldPoint{std::numeric_limits<double>::quiet_NaN(), 5.0}
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error() == motion::GridError::InvalidOrigin);
    }

    SECTION("Origin Y is infinity") {
        const auto result = motion::OccupancyGrid::Create(
            10U,
            20U,
            0.5,
            motion::WorldPoint{5.0, std::numeric_limits<double>::infinity()}
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error() == motion::GridError::InvalidOrigin);
    }

    SECTION("Origin Y is negative infinity") {
        const auto result = motion::OccupancyGrid::Create(
            10U,
            20U,
            0.5,
            motion::WorldPoint{5.0, -std::numeric_limits<double>::infinity()}
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error() == motion::GridError::InvalidOrigin);
    }

    SECTION("Origin Y is NaN") {
        const auto result = motion::OccupancyGrid::Create(
            10U,
            20U,
            0.5,
            motion::WorldPoint{5.0, std::numeric_limits<double>::quiet_NaN()}
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error() == motion::GridError::InvalidOrigin);
    }
}

TEST_CASE("OccupancyGrid creation fails for invalid extent","[grid][create]") {
    SECTION("X extent overflows") {
        const auto result = motion::OccupancyGrid::Create(
            2U,
            1U,
            std::numeric_limits<double>::max(),
            motion::WorldPoint{0.0, 0.0}
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error() == motion::GridError::InvalidExtent);
    }

    SECTION("Y extent overflows") {
        const auto result = motion::OccupancyGrid::Create(
            1U,
            2U,
            std::numeric_limits<double>::max(),
            motion::WorldPoint{0.0, 0.0}
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error() == motion::GridError::InvalidExtent);
    }

    SECTION("X extent is too small to advance the upper bound") {
        const auto result = motion::OccupancyGrid::Create(
            10U,
            20U,
            1e-50,
            motion::WorldPoint{1e50, 0.0}
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error() == motion::GridError::InvalidExtent);
    }

    SECTION("Y extent is too small to advance the upper bound") {
        const auto result = motion::OccupancyGrid::Create(
            10U,
            20U,
            1e-50,
            motion::WorldPoint{0.0, 1e50}
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error() == motion::GridError::InvalidExtent);
    }
}