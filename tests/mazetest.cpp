#include <sstream>
#include "tctestpp.h"
#include "position.h"
#include "tile.h"
#include "tilefactory.h"
#include "maze.h"

Maze *readFromString(const std::string &s) {
  std::stringstream ss(s);
  return Maze::read(ss);
}

const char *m1 =
  "10 6\n"
  "##########\n"
  "#........#\n"
  "#.###....#\n"
  "#.#......#\n"
  "#.....<..#\n"
  "##########\n";

const char *m2_invalid_dims =
    "6\n"
    "##########\n"
    "#........#\n"
    "#.###....#\n"
    "#.#......#\n"
    "#.....<..#\n"
    "##########\n";

const char *m3_illegal_row =
    "10 6\n"
    "##########\n"
    "#........#\n"
    "#.###....#\n"
    "#.#......\n"
    "#.....<..#\n"
    "##########\n";

struct TestObjs {
  Maze *maze1;
  Maze *maze2;
  Maze *maze3;
};

TestObjs *setup() {
  TestObjs *objs = new TestObjs;
  objs->maze1 = readFromString(m1);
  objs->maze2 = readFromString(m2_invalid_dims);
  objs->maze3 = readFromString(m3_illegal_row);  
  return objs;
}

void cleanup(TestObjs *objs) {
  delete objs->maze1;
  delete objs;
}

void testGetWidth(TestObjs *objs);
void testGetHeight(TestObjs *objs);
void testGetTile(TestObjs *objs);
void testSetTile(TestObjs *objs);
void testInvalidMazeDimensions(TestObjs *objs);

int main(int argc, char *argv[]) {
  TEST_INIT();

  // allow test name to be specified on the command line
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST(testGetWidth);
  TEST(testGetHeight);
  TEST(testGetTile);
  TEST(testSetTile);
  TEST(testInvalidMazeDimensions);

  TEST_FINI();
}

void testGetWidth(TestObjs *objs) {
  ASSERT(10 == objs->maze1->getWidth());
}

void testGetHeight(TestObjs *objs) {
  ASSERT(6 == objs->maze1->getHeight());
}

void testGetTile(TestObjs *objs) {
  const Tile *p1 = objs->maze1->getTile(Position(0, 0));
  ASSERT(p1->getGlyph() == "#");
  ASSERT(!p1->isGoal());

  const Tile *p2 = objs->maze1->getTile(Position(1, 1));
  ASSERT(p2->getGlyph() == ".");
  ASSERT(!p2->isGoal());

  const Tile *p3 = objs->maze1->getTile(Position(7, 4));
  ASSERT(!p3->isGoal());
}

void testSetTile(TestObjs *) {
  Maze *maze = new Maze(10, 5);

  Tile *tile1 = TileFactory::getInstance()->createFromChar('#');
  Tile *tile2 = TileFactory::getInstance()->createFromChar('.');
  maze->setTile(Position(0, 0), tile1);
  ASSERT("#" == maze->getTile(Position(0, 0))->getGlyph());
  maze->setTile(Position(4, 3), tile2);
  ASSERT("." == maze->getTile(Position(4, 3))->getGlyph());

  delete maze;
}

void testInvalidMazeDimensions(TestObjs *objs) {
  ASSERT(objs->maze2 == nullptr);
  ASSERT(objs->maze3 == nullptr);
}
