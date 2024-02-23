#include <catch2/catch.hpp>
#include <histogram.h>

using idealgas::Histogram;
using std::vector;
using std::pair;
using glm::vec2;

TEST_CASE("Test Update") {
  vector<float> initial_velocities = {1, 1, 1};
  vector<float> new_velocities = {2, 2, 2};
  Histogram h = Histogram(initial_velocities, 2, 1, 1);
  h.Update(new_velocities);
  REQUIRE(h.GetVelocities() == vector<float>{2,2,2});
}

TEST_CASE("Test SetUp") {
  vector<float> velocities = {1, 1};
  vector<float> unsorted = {3, 5, 1, 2};
  SECTION("Velocity distribution of one segment") {
    Histogram h = Histogram(velocities, 2, 1, 1);
    h.SetUp();
    REQUIRE(h.GetVelocityDistribution() == vector<pair<int, int>>{pair<int, int>(0, 0)});
  }

  SECTION("Velocity distribution of multiple segments") {
    Histogram h = Histogram(velocities, 2, 1, 2);
    h.SetUp();
    REQUIRE(h.GetVelocityDistribution() == vector<pair<int, int>>{pair<int, int>(0,0), pair<int, int>(1,0)});
  }

  SECTION("Bar range with one segment") {
    Histogram h = Histogram(velocities, 2, 1, 1);
    h.SetUp();
    REQUIRE(h.GetBarRange() == 1.0);
  }

  SECTION("Bar range with multiple segments") {
    Histogram h = Histogram(velocities, 4, 1, 3);
    h.SetUp();
    REQUIRE(h.GetBarRange() == 1.0);
  }

  SECTION("Test sort") {
    Histogram h = Histogram(unsorted, 5, 1, 1);
    h.SetUp();
    REQUIRE(h.GetVelocities() == vector<float> {1, 2, 3, 5});
  }
}

TEST_CASE("Test FindVelocityDistribution") {
  vector<float> velocities = {1, 1, 1, 1};
  vector<float> velocities2 = {1, 2, 3, 4};

  SECTION("One segment") {
    Histogram h = Histogram(velocities, 2, 1, 1);
    h.SetUp();
    h.FindVelocityDistribution();
    REQUIRE(h.GetVelocityDistribution() == vector<pair<int, int>>{pair<int, int>(0, 4)});
  }

  SECTION("Multiple segments") {
    SECTION("All data in one bar") {
      Histogram h = Histogram(velocities, 4, 1, 2);
      h.SetUp();
      h.FindVelocityDistribution();
      REQUIRE(h.GetVelocityDistribution() == vector<pair<int, int>>{pair<int, int>(0, 4), pair<int, int>(1, 0)});
    }

    SECTION("Skipped first bar") {
      Histogram h = Histogram(velocities, 1.5, 0, 2);
      h.SetUp();
      h.FindVelocityDistribution();
      REQUIRE(h.GetVelocityDistribution() == vector<pair<int, int>>{pair<int, int>(0, 0), pair<int, int>(1, 4)});
    }

    SECTION("Skipping middle bar") {
      vector<float> velocities3 = {1, 1, 4, 4};
      Histogram h = Histogram(velocities3, 4, 1, 3);
      h.SetUp();
      h.FindVelocityDistribution();
      REQUIRE(h.GetVelocityDistribution() == vector<pair<int, int>>{pair<int, int>(0, 2), pair<int, int>(1, 0), pair<int, int>(2, 2)});
    }

    SECTION("Skipping last bar") {
      Histogram h = Histogram(velocities, 2, 0, 2);
      h.SetUp();
      h.FindVelocityDistribution();
      REQUIRE(h.GetVelocityDistribution() == vector<pair<int, int>>{pair<int, int>(0, 4), pair<int, int>(1, 0)});
    }
  }
}
