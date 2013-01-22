#include <darc/component.hpp>
#include <darc/registry.hpp>
#include <iris/glog.hpp>

int main(int argc, const char** argv)
{
  iris::glog<iris::Info>("Autostart-Peer Running");
  darc::component_manager mngr;

  bool port_found = false;
  for(int port = 5000; port < 5010; port++)
  {
    std::string url = std::string("zmq+tcp://127.0.0.1:").append(boost::to_string(port));
    if(port_found == false)
    {
      try
      {
        mngr.accept(url);
        port_found = true;
      }
      catch(std::exception& e) // todo: only catch address in use exception
      {
        mngr.connect(url);
      }
    }
    else
    {
      mngr.connect(url);
    }
  }

  assert(port_found);

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
