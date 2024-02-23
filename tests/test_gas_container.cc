#include <catch2/catch.hpp>

#include <gas_container.h>

using idealgas::GasContainer;
using idealgas::Particle;
using glm::vec2;
using std::pair;
using std::string;
using std::vector;

TEST_CASE("Test update frame") {
  Particle particle = Particle(1, 1, 1, 1, "black", 1.0, 0.5);
  vector<Particle> particles = vector<Particle>();
  particles.push_back(particle);
  GasContainer container = GasContainer(100, 100, 0, 0, particles);
  container.AdvanceOneFrame();
  REQUIRE(container.GetParticles().at(0).GetPosition() == vec2(2, 2));
  REQUIRE(container.GetParticles().at(0).GetVelocity() == vec2(1, 1));
}

TEST_CASE("Test collision with wall, moving towards wall") {
  SECTION("Collision with left horizontal wall") {
    Particle particle = Particle(1, 2, -1, 1, "black", 1.0, 1.0);
    vector<Particle> particles = vector<Particle>();
    particles.push_back(particle);
    GasContainer container = GasContainer(100, 100, 0, 0, particles);
    container.AdvanceOneFrame();
    REQUIRE(container.GetParticles().at(0).GetVelocity() == vec2(1, 1));
  }

  SECTION("Collision with right horizontal wall") {
    Particle particle = Particle(99, 2, 1, 1, "black", 1.0, 1.0);
    vector<Particle> particles = vector<Particle>();
    particles.push_back(particle);
    GasContainer container = GasContainer(100, 100, 0, 0, particles);
    container.AdvanceOneFrame();
    REQUIRE(container.GetParticles().at(0).GetVelocity() == vec2(-1, 1));
  }

  SECTION("Collision with upper vertical wall") {
    Particle particle = Particle(2, 1, 1, -1, "black", 1.0, 1.0);
    vector<Particle> particles = vector<Particle>();
    particles.push_back(particle);
    GasContainer container = GasContainer(100, 100, 0, 0, particles);
    container.AdvanceOneFrame();
    REQUIRE(container.GetParticles().at(0).GetVelocity() == vec2(1, 1));
  }

  SECTION("Collision with lower vertical wall") {
    Particle particle = Particle(2, 99, 1, 1, "black", 1.0, 1.0);
    vector<Particle> particles = vector<Particle>();
    particles.push_back(particle);
    GasContainer container = GasContainer(100, 100, 0, 0, particles);
    container.AdvanceOneFrame();
    REQUIRE(container.GetParticles().at(0).GetVelocity() == vec2(1, -1));
  }

  SECTION("Corner collision") {
    Particle particle = Particle(1, 1, -1, -1, "black", 1.0, 1.0);
    vector<Particle> particles = vector<Particle>();
    particles.push_back(particle);
    GasContainer container = GasContainer(100, 100, 0, 0, particles);
    container.AdvanceOneFrame();
    REQUIRE(container.GetParticles().at(0).GetVelocity() == vec2(1, 1));
  }
}

TEST_CASE("Test collision with wall, moving away from wall") {
    Particle particle = Particle(1, 2, 1, 1, "black", 1.0, 1.0);
    vector<Particle> particles = vector<Particle>();
    particles.push_back(particle);
    GasContainer container = GasContainer(100, 100, 0, 0, particles);
    container.AdvanceOneFrame();
    REQUIRE(container.GetParticles().at(0).GetVelocity() == vec2(1, 1));
}

TEST_CASE("Test collisions between particles") {
  SECTION("2 particles collide perpendicularly") {
    Particle particle = Particle(vec2(4, 2), vec2(-1, 0), "black", 1.0, 1.0);
    Particle particle2 = Particle(vec2(2, 2), vec2(1, 0), "black", 1.0, 1.0);
    vector<Particle> particles = vector<Particle>();
    particles.push_back(particle);
    particles.push_back(particle2);
    GasContainer container = GasContainer(100, 100, 0, 0, particles);
    container.AdvanceOneFrame();
    REQUIRE(container.GetParticles().at(0).GetVelocity() == vec2(1, 0));
    REQUIRE(container.GetParticles().at(1).GetVelocity() == vec2(-1, 0));
  }

  SECTION("2 particles collide non-perpendicularly") {
    Particle particle = Particle(vec2(2, 2), vec2(1, 0), "black", 1.0, 1.0);
    Particle particle2 = Particle(vec2(4, 2), vec2(-3, -4), "black", 1.0, 1.0);
    vector<Particle> particles = vector<Particle>();
    particles.push_back(particle);
    particles.push_back(particle2);
    GasContainer container = GasContainer(100, 100, 0, 0, particles);
    container.AdvanceOneFrame();
    REQUIRE(container.GetParticles().at(0).GetVelocity() == vec2(-3, 0));
    REQUIRE(container.GetParticles().at(1).GetVelocity() == vec2(1, -4));
  }

  SECTION("3 particles collide") {
    Particle particle = Particle(vec2(4, 2), vec2(-1, 0), "black", 1.0, 1.0);
    Particle particle2 = Particle(vec2(2, 2), vec2(1, 1), "black", 1.0, 1.0);
    Particle particle3 = Particle(vec2(2, 4), vec2(0, -1), "black", 1.0, 1.0);
    vector<Particle> particles = vector<Particle>();
    particles.push_back(particle);
    particles.push_back(particle2);
    particles.push_back(particle3);
    GasContainer container = GasContainer(100, 100, 0, 0, particles);
    container.AdvanceOneFrame();
    REQUIRE(container.GetParticles().at(0).GetVelocity() == vec2(1, 0));
    REQUIRE(container.GetParticles().at(1).GetVelocity() == vec2(-1, -1));
    REQUIRE(container.GetParticles().at(2).GetVelocity() == vec2(0, 1));
  }
}

TEST_CASE("Test GetVelocitiesOfParticleColor") {
  Particle particle = Particle(vec2(4, 2), vec2(1, 0), "black", 1.0, 1.0);
  Particle particle2 = Particle(vec2(2, 2), vec2(1, 0), "black", 1.0, 1.0);
  Particle particle3 = Particle(vec2(2, 4), vec2(1, 0), "white", 1.0, 1.0);
  vector<Particle> particles = vector<Particle>();
  particles.push_back(particle);
  particles.push_back(particle2);
  particles.push_back(particle3);
  GasContainer container = GasContainer(100, 100, 0, 0, particles);
  REQUIRE(container.GetVelocitiesOfParticleColor("black") == vector<float>{1, 1});
}
