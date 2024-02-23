#include "gas_container.h"

namespace idealgas {

GasContainer::GasContainer() {
  container_length_ = kDefaultLength;
  container_height_ = kDefaultHeight;
  margins_left_ = kDefaultLeftMargins;
  margins_top_ = kDefaultTopMargins;
  particles_ = vector<Particle>();
  paused_ = false;
  //https://www.geeksforgeeks.org/rand-and-srand-in-ccpp/
  srand(static_cast<unsigned int>(time(0)));

  GenerateParticles(kDefaultNumParticles, kDefaultNumParticles, kDefaultNumParticles);
  SetUpHistograms();
}

GasContainer::GasContainer(int length, int height, int margins_left, int margins_top, vector<Particle> particles) :
                          container_length_(length), container_height_(height), margins_left_(margins_left),
                          margins_top_(margins_top), particles_(move(particles)) {
  paused_ = false;
  SetUpHistograms();
}

void GasContainer::Display() const {
  //draw the particles
  for (size_t i = 0; i < particles_.size(); i++) {
    particles_.at(i).DrawParticle();
  }

  //draw the container
  ci::gl::color(ci::Color("white"));
  ci::gl::drawStrokedRect(ci::Rectf(vec2(margins_left_, margins_top_),
                                    vec2(container_length_ + margins_left_, container_height_ + margins_top_)));

  //draw the histograms
  white_histogram_.DrawHistogram(vec2(margins_left_ * .1, margins_top_ + container_height_ * .3));
  blue_histogram_.DrawHistogram(vec2(margins_left_ * .1, margins_top_ + container_height_ * .65));
  red_histogram_.DrawHistogram(vec2(margins_left_ * .1, margins_top_ + container_height_));
}

void GasContainer::AdvanceOneFrame() {
  if (!paused_) {
    HandleAllCollisions();
    for (size_t i = 0; i < particles_.size(); i++) {
      particles_.at(i).UpdateParticle();
    }
    UpdateHistograms();
  }
}

void GasContainer::HandleAllCollisions() {
  for (size_t i = 0; i < particles_.size(); i++) {
    Particle current_particle = particles_.at(i);
    float current_x = current_particle.GetPosition().x;
    float current_y = current_particle.GetPosition().y;
    float current_radius = current_particle.GetRadius();
    for (size_t j = i + 1; j < particles_.size(); j++) {

      //check for collisions with other particles
      if (current_particle.HasCollided(particles_.at(j))) {
        pair<vec2, vec2> new_velocities = current_particle.GetVelocitiesAfterCollision(particles_.at(j));
        particles_.at(i).SetVelocity(new_velocities.first);
        particles_.at(j).SetVelocity(new_velocities.second);
        velocities_.at(j) = glm::length(new_velocities.second);
      }
    }

    //check for collisions with horizontal walls
    if ((current_x - current_radius <= margins_left_ && current_particle.GetVelocity().x < 0)
        || (current_x + current_radius >= container_length_ + margins_left_ && current_particle.GetVelocity().x > 0)) {
      particles_.at(i).HandleHorizontalWallCollision();
    }

    //check for collisions with vertical walls
    if ((current_y - current_radius <= margins_top_ && current_particle.GetVelocity().y < 0)
        || (current_y + current_radius >= container_height_ + margins_top_ && current_particle.GetVelocity().y > 0)) {
      particles_.at(i).HandleVerticalWallCollision();
    }

    velocities_.at(i) = glm::length(particles_.at(i).GetVelocity());
  }
}

void GasContainer::GenerateParticles(int num_white_particles, int num_blue_particles, int num_red_particles) {
  GenerateWhiteParticles(num_white_particles);
  GenerateBlueParticles(num_blue_particles);
  GenerateRedParticles(num_red_particles);

  max_velocity_ = *std::max_element(velocities_.begin(), velocities_.end());
  min_velocity_ = *std::min_element(velocities_.begin(), velocities_.end());
}

void GasContainer::GenerateWhiteParticles(int num_particles) {
  Particle p_white = kWhiteParticle;
  for (int i = 0; i < num_particles; i++) {
    p_white.InitializeParticle(container_length_, container_height_, margins_left_, margins_top_);
    particles_.push_back(p_white.Copy());
    velocities_.push_back(glm::length(p_white.GetVelocity()));
  }
}

void GasContainer::GenerateBlueParticles(int num_particles) {
  Particle p_blue = kBlueParticle;
  for (int i = 0; i < num_particles; i++) {
    p_blue.InitializeParticle(container_length_, container_height_, margins_left_, margins_top_);
    particles_.push_back(p_blue.Copy());
    velocities_.push_back(glm::length(p_blue.GetVelocity()));
  }
}

void GasContainer::GenerateRedParticles(int num_particles) {
  Particle p_red = kRedParticle;
  for (int i = 0; i < num_particles; i++) {
    p_red.InitializeParticle(container_length_, container_height_, margins_left_, margins_top_);
    particles_.push_back(p_red.Copy());
    velocities_.push_back(glm::length(p_red.GetVelocity()));
  }
}

vector<float> GasContainer::GetVelocitiesOfParticleColor(const string &color) {
  vector<float> to_return = vector<float>();
  for (size_t i = 0; i < particles_.size(); i++) {
    if (particles_.at(i).GetColor() == color) {
      to_return.push_back(glm::length(particles_.at(i).GetVelocity()));
    }
  }
  return to_return;
}

void GasContainer::SetUpHistograms() {
  int length = int(margins_left_ * .8);
  int height = int(container_height_ * .3);
  int segments = 10;
  white_histogram_ = Histogram("white", length, height, max_velocity_, min_velocity_,
                               GetVelocitiesOfParticleColor("white"), segments);
  blue_histogram_ = Histogram("blue", length, height, max_velocity_, min_velocity_,
                              GetVelocitiesOfParticleColor("blue"), segments);
  red_histogram_ = Histogram("red", length, height, max_velocity_, min_velocity_,
                             GetVelocitiesOfParticleColor("red"), segments);
  white_histogram_.SetUp();
  blue_histogram_.SetUp();
  red_histogram_.SetUp();
}

void GasContainer::UpdateHistograms() {
  max_velocity_ = *std::max_element(velocities_.begin(), velocities_.end());
  min_velocity_ = *std::min_element(velocities_.begin(), velocities_.end());
  white_histogram_.Update(GetVelocitiesOfParticleColor("white"), max_velocity_, min_velocity_);
  white_histogram_.FindVelocityDistribution();
  blue_histogram_.Update(GetVelocitiesOfParticleColor("blue"), max_velocity_, min_velocity_);
  blue_histogram_.FindVelocityDistribution();
  red_histogram_.Update(GetVelocitiesOfParticleColor("red"), max_velocity_, min_velocity_);
  red_histogram_.FindVelocityDistribution();
}

vector<Particle> GasContainer::GetParticles() {
  return particles_;
}

bool GasContainer::GetPaused() {
  return paused_;
}

void GasContainer::SetPaused(bool paused) {
  paused_ = paused;
}

}  // namespace idealgas
