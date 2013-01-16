#include <darc/component.hpp>
#include <darc/registry.hpp>
#include <iris/glog.hpp>

int main(int argc, const char** argv)
{
  iris::glog<iris::Info>("Autostart-Peer Running");
  darc::component_manager mngr;
  mngr.accept("zmq+tcp://127.0.0.1:5001");
//  mngr.connect("zmq+tcp://127.0.0.1:5002");

  std::vector<std::string> names = darc::Registry::names();
  for(std::vector<std::string>::iterator it = names.begin();
      it != names.end();
      it++)
  {
    darc::component_ptr c = darc::Registry::instantiateComponent(*it, &mngr);
    c->run();
  }

  mngr.run_current_thread();
}
