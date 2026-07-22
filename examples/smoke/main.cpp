import motion;
import motion.grid;


int main() {
  static_assert(motion::version_major == 0);
  static_assert(motion::version_minor == 1);
  static_assert(motion::version_patch == 0);

  const auto grid = motion::OccupancyGrid::Create(10U, 20U, 0.5, motion::WorldPoint{0.0, 0.0});

  std::vector<int>(9);

  


  //const auto current = grid->GetCellState(motion::GridCell(4U, 3U));



  if (!grid.has_value()) {
    return 1;
  }

  return 0;
} 
