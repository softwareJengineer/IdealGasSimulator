#pragma once

#include "cinder/gl/gl.h"
#include "particle.h"
#include "histogram.h"
#include <utility>

namespace idealgas {

using std::vector;
using glm::vec2;
using std::string;

/**
 * The container in which all of the gas particles are contained. This class
 * stores all of the particles and updates them on each frame of the simulation.
 */
class GasContainer {
 public:

  GasContainer();

  /**
   * GasContainer constructor
   * @param width width of the container
   * @param length length of the container
   * @param particles the particles in the container
   */
  GasContainer(int length, int height, int margins_left, int margins_top, vector<Particle> particles);

  /**
   * Displays the container walls and the current positions of the particles.
   */
  void Display() const;

  /**
   * Updates the positions and velocities of all particles (based on the rules
   * described in the assignment documentation).
   */
  void AdvanceOneFrame();

  vector<Particle> GetParticles();

  vector<float> GetVelocitiesOfParticleColor(const string& color);


  /**
   * Creates the histogram objects and sets them up
   */
  void SetUpHistograms();

  bool GetPaused();

  void SetPaused(bool paused);

 private:
    int container_height_;
    int container_length_;
    int margins_top_;
    int margins_left_;
    float max_velocity_;
    float min_velocity_;
    Histogram white_histogram_;
    Histogram blue_histogram_;
    Histogram red_histogram_;

    //if the simulation is paused or not
    bool paused_;

    static const int kDefaultNumParticles = 50;
    static const int kDefaultLength = 750;
    static const int kDefaultHeight = 750;
    static const int kDefaultTopMargins = 100;
    static const int kDefaultLeftMargins = 300;

    const Particle kWhiteParticle = Particle("white", 1.0, 5.0);
    const Particle kBlueParticle = Particle("blue", 3.0, 8.0);
    const Particle kRedParticle = Particle("red", 5.0, 10.0);

    /**
     * The vector we store particles in
     */
    vector<Particle> particles_;

    /**
     * Vector storing the velocity of all the particles
     */
    vector<float> velocities_;

    /**
     * Creates random particles and puts them into particles_
     * @param num_particles number of particles to generate
     */
    void GenerateParticles(int num_blue_particles, int num_white_particles, int num_red_particles);

    void GenerateWhiteParticles(int num_particles);

    void GenerateBlueParticles(int num_particles);

    void GenerateRedParticles(int num_particles);

    /**
     * Handles all collisions
     */
    void HandleAllCollisions();

    /**
     * Will update histograms with new velocities, max and min velocities
     */
    void UpdateHistograms();
};

}  // namespace idealgas
