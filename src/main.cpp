#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include <random>
#include <vector>

std::vector<std::vector<bool>> new_grid();
void update_grid(std::vector<std::vector<bool>> &grid);
const unsigned int CELL_SIZE = 15;
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
int main() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 0;
  sf::RenderWindow window = sf::RenderWindow{
      sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "CMake SFML Project"};
  window.setFramerateLimit(10);
  auto grid = new_grid();

  while (window.isOpen()) {
    for (auto event = sf::Event{}; window.pollEvent(event);) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear(sf::Color::Black);
    for (int x = 0; x < WINDOW_WIDTH / CELL_SIZE; x++) {
      for (int y = 0; y < WINDOW_HEIGHT / CELL_SIZE; y++) {
        if (grid[y][x]) {
          sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
          cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
          cell.setFillColor(sf::Color::White);
          window.draw(cell);
        }
      }
    }
    window.display();
    update_grid(grid);
  }
}

bool generate_random_bool() {
  std::random_device rd; // Seed for the random number generator
  std::mt19937 gen(
      rd()); // Mersenne Twister engine for generating random numbers
  std::bernoulli_distribution d(0.5); // 50% chance of true or false

  return d(gen); // Generate and return the random boolean value
}

std::vector<std::vector<bool>> new_grid() {
  std::vector<std::vector<bool>> grid;
  for (int y = 0; y < WINDOW_HEIGHT / CELL_SIZE; y++) {
    std::vector<bool> row;
    for (int x = 0; x < WINDOW_WIDTH / CELL_SIZE; x++) {
      bool rand = generate_random_bool();
      row.push_back(rand);
    }
    grid.push_back(row);
  }
  return grid;
}

int count_neighbors(std::vector<std::vector<bool>> grid, int x, int y) {
  int count = 0;
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      if (dx == 0 && dy == 0)
        continue;
      int nx = x + dx;
      int ny = y + dy;
      if (nx >= 0 && nx < grid[0].size() && ny >= 0 && ny < grid.size() &&
          grid[ny][nx]) {
        count++;
      }
    }
  }
  return count;
}

void update_grid(std::vector<std::vector<bool>> &grid) {
  auto new_grid = grid;
  int rows = WINDOW_HEIGHT / CELL_SIZE;
  int cols = WINDOW_WIDTH / CELL_SIZE;

  for (int x = 0; x < cols; x++) {
    for (int y = 0; y < rows; y++) {
      int neighbors = count_neighbors(grid, x, y);
      if (grid[y][x]) {
        if (neighbors < 2 || neighbors > 3) {
          new_grid[y][x] = false;
        }
      } else if (neighbors == 3) {
        new_grid[y][x] = true;
      }
    }
  }
  grid = new_grid;
}
