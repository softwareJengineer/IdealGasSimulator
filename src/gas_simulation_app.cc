#include "gas_simulation_app.h"

namespace idealgas {

IdealGasApp::IdealGasApp() {
  ci::app::setWindowSize(kWindowSize, kWindowSize);
}

void IdealGasApp::draw() {
  ci::Color background_color("black");
  ci::gl::clear(background_color);

  container_.Display();
}

void IdealGasApp::update() {
  container_.AdvanceOneFrame();
}

void IdealGasApp::keyUp(KeyEvent event) {
  if (event.getCode() == KeyEvent::KEY_SPACE) {
    container_.SetPaused(!container_.GetPaused());
  }
}

}  // namespace idealgas
