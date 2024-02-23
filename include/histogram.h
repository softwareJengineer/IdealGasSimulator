#pragma once

#include "cinder/gl/gl.h"
#include "particle.h"

namespace idealgas {

using std::vector;

class Histogram {
 public:

  Histogram();

  /**
   * Histogram constructor
   * @param color
   * @param length
   * @param height
   * @param max_velocity max velocity of all particles in container
   * @param min_velocity min velocity of all particles in container
   * @param velocities list of velocities to be displayed by this histogram
   * @param num_segments number of bars/segments to be displayed
   */
  Histogram(const string& color, int length, int height, float max_velocity,
            float min_velocity, const vector<float>& velocities, int num_segments);

  //for testing
  Histogram(const vector<float>& velocities, float max_velocity, float min_velocity, int num_segments);

  /**
   * Updates the histogram with new velocities. For testing
   * @param new_velocities
   */
  void Update(const vector<float>& new_velocities);

  /**
   * Updates the histograms with new velocities, max velocity, and min velocity.
   * @param new_velocities
   * @param max_velocity
   * @param min_velocity
   */
  void Update(const vector<float>& new_velocities, float max_velocity, float min_velocity);

  /**
   * Draws the histogram
   * @param corner bottom left corner of the histogram
   */
  void DrawHistogram(const vec2& corner) const;

  /**
   * Sets up the velocity_distribution
   */
  void SetUp();

  /**
   * Gets the frequency of velocities and puts them in velocity_distribution_
   */
  void FindVelocityDistribution();

  vector<float> GetVelocities();

  float GetBarRange();

  vector<pair<int, int>> GetVelocityDistribution();

 private:
  string color_;
  int length_;
  int height_;
  float max_velocity_;
  float min_velocity_;
  int num_segments_;

  //a list of the velocities to be displayed in this histogram
  vector<float> velocities_;

  //the velocity range of each bar on the histogram
  float bar_range_;

  //vector matching bar number to the number of velocities in it
  vector<pair<int, int>> velocity_distribution_;

};

}