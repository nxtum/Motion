#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_test_macros.hpp>

import motion;

TEST_CASE( "Valid Occupancy can be created", "[grid]" ) {
    const auto result = motion::OccupancyGrid::Create(
        10U,
        20U,
        0.5,
        motion::WorldPoint{1.0, 2.0}
    );

    REQUIRE(result.has_value());
    const auto& grid = *result;

    CHECK(grid.Width() == 10U);
    CHECK(grid.Height() == 20U);
    CHECK_THAT(grid.Resolution(), Catch::Matchers::WithinAbs(0.5, 0.01));
    CHECK_THAT(grid.Origin().x, Catch::Matchers::WithinAbs(1.0, 0.01));
    CHECK_THAT(grid.Origin().y, Catch::Matchers::WithinAbs(2.0, 0.01));
}