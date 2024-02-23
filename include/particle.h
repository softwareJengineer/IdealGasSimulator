#pragma once

#include "cinder/gl/gl.h"

namespace idealgas {

using glm::vec2;
using std::string;
using std::pair;

class Particle {
 public:

  /**
   * Default Particle constructor. Sets color, mass, and radius automatically.
   */
  Particle();

  /**
   * General Particle object constructor
   * @param position position vector
   * @param velocity velocity vector
   * @param color
   * @param mass
   * @param radius
   */
  Particle(const vec2& position, const vec2& velocity, const string color, const float& mass, const float& radius);

  /**
   * Another general Particle object constructor
   * @param x_pos x position of particle
   * @param y_pos y position of particle
   * @param x_velocity
   * @param y_velocity
   * @param color
   * @param mass
   * @param radius
   */
  Particle(const float& x_pos, const float& y_pos, const float& x_velocity,
           const float& y_velocity, const string color, const float& mass, const float& radius);

  Particle(const string& color, const float& mass, const float& radius);

  /**
   * Updates the particle's position based on its velocity
   */
  void UpdateParticle();

  /**
   * Calculates both particles' new velocities after colliding
   * @param other the particle being collided with
   * @return a pair of both new velocities, with this particle's new velocity being first
   */
  pair<vec2, vec2> GetVelocitiesAfterCollision(const Particle &other);

  /**
   * Changes this particle's velocity to handle colliding with a vertical wall
   */
  void HandleVerticalWallCollision();

  /**
   * Changes this particle's velocity to handle colliding with a horizontal wall
   */
  void HandleHorizontalWallCollision();

  vec2 GetPosition() const;

  vec2 GetVelocity() const;

  void SetVelocity(const vec2& new_velocity);

  string GetColor() const;

  float GetMass() const;

  float GetRadius() const;

  /**
   * Calculates if two particles have collided based off of radius and position
   * @param other the particle we are checking to see if this particle has collided with
   * @return if the particles have collided
   */
  bool HasCollided(const Particle& other);

  /**
   * Will set this particle's initial position, velocity randomly based off container size
   * and will set its size and color
   * @param container_length
   * @param container_height
   * @param margins_left
   * @param margins_top
   * @param color
   */
  void InitializeParticle(int container_length, int container_height, int margins_left, int margins_top);

  /**
   * Makes a copy of this particle
   * @return the copy
   */
  Particle Copy();

  void DrawParticle() const;

 private:
  vec2 position_;
  vec2 velocity_;
  string color_;
  float mass_;
  float radius_;

  /**
   * Calculates a particle's new velocity after a collision
   * @param velocity1 particle 1 velocity
   * @param velocity2 particle 2 velocity
   * @param position1 particle 1 position
   * @param position2 particle 2 position
   * @param mass1 particle 1 mass
   * @param mass2 particle 2 mass
   * @return a vector of this particle's new velocity
   */
  vec2 GetNewVelocity(const vec2& velocity1, const vec2& velocity2, const vec2& position1,
                      const vec2& position2, const float& mass1, const float& mass2);

};

} //namespace idealgas