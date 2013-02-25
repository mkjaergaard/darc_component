#include <darc/component_manager.hpp>
#include <iris/glog.hpp>

#include "my_component1.cpp"

int main(int argc, const char** argv)
{
  iris::glog<iris::Info>("Node Running");
  darc::component_manager mngr;
  mngr.accept("zmq+tcp://127.0.0.1:5001");
  mngr.connect("zmq+tcp://127.0.0.1:5002");

  darc::component_ptr c1 = darc::component::instantiate<my_component1>("myc", &mngr);
  c1->run();

  mngr.run_current_thread();
}
