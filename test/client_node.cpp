#include <darc/component_manager.hpp>
#include <beam/glog.hpp>

#include "my_client_component.cpp"
#include "my_server_component.cpp"

int main(int argc, const char** argv)
{
  beam::glog<beam::Info>("Node Running");
  darc::component_manager mngr;
  mngr.accept("zmq+tcp://127.0.0.1:5001");
  mngr.connect("zmq+tcp://127.0.0.1:5002");

  darc::component_ptr c1 = darc::component::instantiate<my_client_component>("my_client", &mngr);

  darc::component_ptr c2 = darc::component::instantiate<my_server_component>("my_server", &mngr);
  c1->run();
  c2->run();

  mngr.run_current_thread();
}
