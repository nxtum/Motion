#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_test_macros.hpp>

#include <limits>
import motion;

TEST_CASE( "OccupancyGrid creation succeeds for valid inputs","[grid][create]" ) {
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

TEST_CASE( "OccupancyGrid creation fails for invalid dimensions","[grid][create]" ) {
    const auto result = motion::OccupancyGrid::Create(
        0U,
        20U,
        0.5,
        motion::WorldPoint{1.0, 2.0}
    );
    REQUIRE_FALSE(result.has_value());
    CHECK(result.error() == motion::GridError::InvalidDimensions);
}

TEST_CASE( "OccupancyGrid creation fails for invalid resolution","[grid][create]" ) {
    const auto result = motion::OccupancyGrid::Create(
        10U,
        20U,
        -3.5,
        motion::WorldPoint{1.0, 2.0}
    );
    REQUIRE_FALSE(result.has_value());
    CHECK(result.error() == motion::GridError::InvalidResolution);
}

TEST_CASE( "OccupancyGrid creation fails for invalid origin","[grid][create]" ) {
    const auto result = motion::OccupancyGrid::Create(
        10U,
        20U,
        0.5,
        motion::WorldPoint{std::numeric_limits<double>::infinity(),5.0}
    );
    REQUIRE_FALSE(result.has_value());
    CHECK(result.error() == motion::GridError::InvalidOrigin);
}

// The X extent is too small to advance the upper bound at this origin
TEST_CASE( "OccupancyGrid creation fails for invalid extent","[grid][create]" ) {
    const auto result = motion::OccupancyGrid::Create(
        10U,
        20U,
        1e-50,
        motion::WorldPoint{1e50 ,5.0}
    );
    REQUIRE_FALSE(result.has_value());
    CHECK(result.error() == motion::GridError::InvalidExtent);
}