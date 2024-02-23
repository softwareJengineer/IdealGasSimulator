#include <particle.h>

#include <utility>

namespace idealgas {

Particle::Particle() {
  position_ = vec2(1.0, 1.0);
  velocity_ = vec2(1.0, 1.0);
  color_ = "white";
  mass_ = 1.0;
  radius_ = 1.0;
}

Particle::Particle(const vec2& position, const vec2& velocity,
                   const string color, const float& mass, const float& radius) :
                  position_(position), velocity_(velocity), color_(color), mass_(mass), radius_(radius) {
}

Particle::Particle(const float& x_pos, const float& y_pos, const float& x_velocity,
                   const float& y_velocity, const string color, const float& mass, const float&radius) :
      color_(color), mass_(mass), radius_(radius) {
  position_ = vec2(x_pos, y_pos);
  velocity_ = vec2(x_velocity, y_velocity);
}

Particle::Particle(const string& color, const float& mass, const float& radius) : color_(color), mass_(mass), radius_(radius) {}

void Particle::UpdateParticle() {
  position_ += velocity_;
}

pair<vec2, vec2> Particle::GetVelocitiesAfterCollision(const Particle& other) {
  if (glm::dot((velocity_ - other.GetVelocity()),
               (position_ - other.GetPosition())) < 0) {
    vec2 v1_prime = GetNewVelocity(velocity_, other.GetVelocity(), position_, other.GetPosition(), mass_, other.GetMass());
    vec2 v2_prime = GetNewVelocity(other.GetVelocity(), velocity_, other.GetPosition(), position_, other.GetMass(), mass_);
    return pair<vec2, vec2>(v1_prime, v2_prime);
  }
  return pair<vec2, vec2>(velocity_, other.GetVelocity());
}

void Particle::HandleVerticalWallCollision() {
  SetVelocity(vec2(velocity_.x, velocity_.y * -1));
}

void Particle::HandleHorizontalWallCollision() {
  SetVelocity(vec2(velocity_.x * -1, velocity_.y));
}

void Particle::InitializeParticle(int container_length, int container_height, int margins_left, int margins_top) {
  //sets position to some random value within the container, outside margins
  position_ = vec2((rand() % int(container_length - 2 * radius_))
                       + radius_ + margins_left, (rand() % int(container_height - 2 * radius_)) + margins_top + radius_);
  //sets velocity to somewhere between -radius and +radius
  velocity_ = vec2((rand() % int(radius_) - int(radius_ / 2)), (rand() % int(radius_)) - int(radius_ / 2));
}

vec2 Particle::GetNewVelocity(const vec2& velocity1,
                              const vec2& velocity2,
                              const vec2& position1,
                              const vec2& position2,
                              const float& mass1,
                              const float& mass2) {
  return velocity1 - (((2 * mass2) / (mass1 + mass2)) * (glm::dot((velocity1 - velocity2),(position1 - position2))
                       / (glm::length(position1 - position2) * glm::length(position1 - position2)))
                      * (position1 - position2));
}

bool Particle::HasCollided(const Particle& other) {
  if (glm::distance(position_, other.GetPosition()) <= radius_ + other.GetRadius()) {
    return true;
  }
  return false;
}

void Particle::SetVelocity(const vec2& new_velocity) {
  velocity_ = new_velocity;
}

Particle Particle::Copy() {
  return Particle(position_, velocity_, color_, mass_, radius_);
}

void Particle::DrawParticle() const {
  ci::gl::color(ci::Color(color_.c_str()));
  ci::gl::drawSolidCircle(position_, radius_);
}

vec2 Particle::GetPosition() const {
  return position_;
}

vec2 Particle::GetVelocity() const {
  return velocity_;
}

string Particle::GetColor() const {
  return color_;
}

float Particle::GetMass() const {
  return mass_;
}

float Particle::GetRadius() const {
  return radius_;
}

}