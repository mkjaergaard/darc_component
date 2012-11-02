#include <darc/component_manager.hpp>
#include <beam/glog.hpp>

#include "my_ping_component.cpp"
#include "my_pong_component.cpp"

int main(int argc, const char** argv)
{
  beam::glog<beam::Info>("Node Running");
  darc::component_manager mngr;
  mngr.accept("zmq+tcp://127.0.0.1:5001");
  mngr.connect("zmq+tcp://127.0.0.1:5002");

  darc::ComponentPtr c1 = darc::Component::instantiate<my_ping_component>("myc1", &mngr);
  c1->run();

  darc::ComponentPtr c2 = darc::Component::instantiate<my_pong_component>("myc2", &mngr);
  c2->run();

  mngr.runCurrentThread();
}
