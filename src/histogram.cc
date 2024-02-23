#include "histogram.h"

namespace idealgas {

Histogram::Histogram() {}

Histogram::Histogram(const string& color, int length, int height, float max_velocity,
                     float min_velocity, const vector<float>& velocities, int num_segments) :
                      color_(color), length_(length), height_(height),
                      max_velocity_(max_velocity), min_velocity_(min_velocity), velocities_(velocities),
                      num_segments_(num_segments) {
  if (num_segments < 1 || max_velocity < min_velocity) {
    throw std::invalid_argument("One or more parameters were invalid");
  }
}

Histogram::Histogram(const vector<float>& velocities, float max_velocity, float min_velocity, int num_segments) :
                      velocities_(velocities), max_velocity_(max_velocity), min_velocity_(min_velocity), num_segments_(num_segments) {
  if (num_segments < 1 || max_velocity < min_velocity) {
    throw std::invalid_argument("One or more parameters were invalid");
  }
}

void Histogram::DrawHistogram(const vec2& corner) const {
  if (corner.x < 0 || corner.y < 0) {
    throw std::invalid_argument("Corner cannot be negative.");
  }

  ci::Rectf rectangle = ci::Rectf(corner, vec2(corner.x + float(length_), corner.y - float(height_)));
  ci::gl::color(ci::Color(color_.c_str()));
  ci::gl::drawStrokedRect(rectangle);
  float bar_length = float(length_) / num_segments_;

  for (int i = 0; i < num_segments_; i++) {
    vec2 bottom_left_corner = vec2(corner.x + float(float(i) * bar_length), corner.y);
    vec2 top_right_corner = vec2(corner.x + float(float(i + 1) * bar_length),
                                 corner.y - float(height_ / 30) * float(velocity_distribution_.at(i).second));
    ci::Rectf bar = ci::Rectf(bottom_left_corner, top_right_corner);
    ci::gl::drawSolidRect(bar);
  }

  ci::gl::drawString("Speed", vec2(corner.x, corner.y + 5));
  ci::gl::drawString("F\nr\ne\nq\nu\ne\nn\nc\ny", vec2(corner.x - 10, corner.y - height_ * .75));
}

void Histogram::Update(const vector<float>& new_velocities) {
  velocities_ = new_velocities;
}

void Histogram::Update(const vector<float>& new_velocities, float max_velocity, float min_velocity) {
  velocities_ = new_velocities;
  max_velocity_ = max_velocity;
  min_velocity_ = min_velocity;
}

void Histogram::SetUp() {
  velocity_distribution_ = vector<pair<int, int>>();
  for (int i = 0; i < num_segments_; i++) {
    velocity_distribution_.emplace_back(i, 0);
  }
  bar_range_ = (max_velocity_ - min_velocity_) / float(num_segments_);
  std::sort(velocities_.begin(), velocities_.end());
}

void Histogram::FindVelocityDistribution() {
  int current_bar = 0;
  SetUp();
  for (size_t i = 0; i < velocities_.size() && current_bar < num_segments_; i) {
    if (velocities_.at(i) <= (bar_range_ * float(current_bar + 1)) + min_velocity_) {
      velocity_distribution_.at(current_bar).second = velocity_distribution_.at(current_bar).second + 1;
      i++;
    } else {
      current_bar++;
    }
  }
}

vector<float> Histogram::GetVelocities() {
  return velocities_;
}

float Histogram::GetBarRange() {
  return bar_range_;
}

vector<pair<int, int>> Histogram::GetVelocityDistribution() {
  return vector<pair<int, int>>(velocity_distribution_);
}

}