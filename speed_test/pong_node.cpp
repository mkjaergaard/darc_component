#include <darc/component_manager.hpp>
#include <iris/glog.hpp>

#include "my_pong_component.cpp"

int main(int argc, const char** argv)
{
  iris::glog<iris::Info>("Node Running");
  darc::component_manager mngr;
  mngr.accept("zmq+tcp://127.0.0.1:5002");
  mngr.connect("zmq+tcp://127.0.0.1:5001");

  darc::component_ptr c1 = darc::component::instantiate<my_pong_component>("myc1", &mngr);
  c1->run();

  mngr.run_current_thread();
}
