#include <cassert>
#include <iostream>
#include <type_traits>

import motion;

template <typename E, typename = std::enable_if_t<std::is_enum_v<E>>>
    std::ostream& operator<<(std::ostream& os, E e) {
      return os << static_cast<std::underlying_type_t<E>>(e);
    }


int main() {
  static_assert(motion::version_major == 0);
  static_assert(motion::version_minor == 1);
  static_assert(motion::version_patch == 0);

  const auto grid = motion::OccupancyGrid::Create(10U, 20U, 0.5, motion::WorldPoint{0.0, 0.0});


  if (grid.has_value()) {
        std::cout << "Success! Result " << "\n";
    } else {
        std::cout << "Failure: " << grid.error();
    }

 if (grid->StateAt(motion::GridCell{2U, 3U}) == motion::CellState::Unknown) {
    std::cout << "Test" << std::endl;
 };


  return 0;
} 
