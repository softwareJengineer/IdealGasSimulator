#include <catch2/catch.hpp>

#include <particle.h>

using idealgas::Particle;
using std::pair;
using glm::vec2;

TEST_CASE("Test UpdateParticle") {
  Particle particle = Particle(vec2(1,  2), vec2(3, 4), "blue", 1.0, 1.0);
  particle.UpdateParticle();
  REQUIRE(particle.GetPosition().x == 4);
  REQUIRE(particle.GetPosition().y == 6);
}

TEST_CASE("Test GetVelocitiesAfterCollision") {
  SECTION("Perpendicular collision") {
    Particle particle = Particle(vec2(0, 0), vec2(-1, 0), "black", 1.0, 1.0);
    Particle particle2 = Particle(vec2(-2, 0), vec2(1, 0), "black", 1.0, 1.0);
    pair<vec2, vec2> new_velocities = particle.GetVelocitiesAfterCollision(particle2);
    REQUIRE(new_velocities.first == vec2(1, 0));
    REQUIRE(new_velocities.second == vec2(-1, 0));
  }

  SECTION("Non-perpendicular collision") {
    Particle particle = Particle(vec2(0, 0), vec2(1, 0), "black", 1.0, 1.0);
    Particle particle2 = Particle(vec2(2, 0), vec2(-3, -4), "black", 1.0, 1.0);
    pair<vec2, vec2> new_velocities = particle.GetVelocitiesAfterCollision(particle2);
    REQUIRE(new_velocities.first == vec2(-3, 0));
    REQUIRE(new_velocities.second == vec2(1, -4));
  }

  SECTION("Already collided, moving towards each other") {
    Particle particle = Particle(vec2(0,0), vec2(1, 0), "black", 1.0, 1.0);
    Particle particle2 = Particle(vec2(1, 0), vec2(-1, 0), "black", 1.0, 1.0);
    pair<vec2, vec2> new_velocities = particle.GetVelocitiesAfterCollision(particle2);
    REQUIRE(new_velocities.first == vec2(-1, 0));
    REQUIRE(new_velocities.second == vec2(1, 0));
  }

  SECTION("Already collided, moving away from each other") {
    Particle particle = Particle(vec2(0,0), vec2(1, 0), "black", 1.0, 1.0);
    Particle particle2 = Particle(vec2(1, 0), vec2(1, 0), "black", 1.0, 1.0);
    pair<vec2, vec2> new_velocities = particle.GetVelocitiesAfterCollision(particle2);
    REQUIRE(new_velocities.first == vec2(1, 0));
    REQUIRE(new_velocities.second == vec2(1, 0));
  }
}

TEST_CASE("Test HandleVerticalWallCollision") {
  Particle particle = Particle(vec2(0, 0), vec2(1, 1), "black", 1.0, 1.0);
  particle.HandleVerticalWallCollision();
  REQUIRE(particle.GetVelocity() == vec2(1, -1));
}

TEST_CASE("Test HandleHorizontalWallCollision") {
  Particle particle = Particle(vec2(0, 0), vec2(1, 1), "black", 1.0, 1.0);
  particle.HandleHorizontalWallCollision();
  REQUIRE(particle.GetVelocity() == vec2(-1, 1));
}

TEST_CASE("Test SetVelocity") {
  Particle particle = Particle(vec2(0, 0), vec2(1, 1), "black", 1.0, 1.0);
  particle.SetVelocity(vec2(2, 2));
  REQUIRE(particle.GetVelocity() == vec2(2, 2));
}

TEST_CASE("Test HasCollided") {
  SECTION("No collision") {
    Particle particle = Particle(vec2(0, 0), vec2(1, 1), "black", 1.0, 1.0);
    Particle particle2 = Particle(vec2(3, 0), vec2(1, 1), "black", 1.0, 1.0);
    REQUIRE(particle.HasCollided(particle2) == false);
  }

  SECTION("Collision at edge of radii") {
    Particle particle = Particle(vec2(0, 0), vec2(1, 1), "black", 1.0, 1.0);
    Particle particle2 = Particle(vec2(2, 0), vec2(1, 1), "black", 1.0, 1.0);
    REQUIRE(particle.HasCollided(particle2) == true);
  }

  SECTION("Collision with 2 different radii") {
    Particle particle = Particle(vec2(0, 0), vec2(1, 1), "black", 1.0, 1.0);
    Particle particle2 = Particle(vec2(3, 0), vec2(1, 1), "black", 1.0, 2.0);
    REQUIRE(particle.HasCollided(particle2) == true);
  }

  SECTION("Particles have already collided") {
    Particle particle = Particle(vec2(0, 0), vec2(1, 1), "black", 1.0, 1.0);
    Particle particle2 = Particle(vec2(1, 0), vec2(1, 1), "black", 1.0, 1.0);
    REQUIRE(particle.HasCollided(particle2) == true);
  }
}