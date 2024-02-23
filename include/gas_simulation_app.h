#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "gas_container.h"
#include "particle.h"

namespace idealgas {

using cinder::app::KeyEvent;

/**
 * An app for visualizing the behavior of an ideal gas.
 */
class IdealGasApp : public ci::app::App {
 public:
  IdealGasApp();

  void draw() override;
  void update() override;

  /**
   * Used currently to pause the simulation on space press
   * @param event the key pressed
   */
  void keyUp(KeyEvent event) override;

  // provided that you can see the entire UI on your screen.
  const int kWindowSize = 1250;
  const int kMargin = 100;

 private:
  GasContainer container_;
};

}  // namespace idealgas
