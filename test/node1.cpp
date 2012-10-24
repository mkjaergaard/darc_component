#include <darc/component_manager.hpp>
#include <beam/glog.hpp>

#include "my_component1.cpp"

int main(int argc, const char** argv)
{
  beam::glog<beam::Info>("Node Running");
  darc::ComponentManager mngr;
//  mngr.accept("zmq+tcp://127.0.0.1:5001");
//  mngr.connect("zmq+tcp://127.0.0.1:5002");

  darc::ComponentPtr c1 = darc::Component::instantiate<my_component1>("myc", &mngr);
  c1->run();

  mngr.runCurrentThread();
}
