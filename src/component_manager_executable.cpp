#include <boost/program_options.hpp>
#include <darc/component.hpp>
#include <darc/registry.hpp>
#include <darc/component_loader.hpp>
#include <iris/glog.hpp>
#include <darc/network/address_in_use_exception.hpp>

namespace po = boost::program_options;

typedef std::vector<std::string> StringVecType;

int main(int argc, const char* argv[])
{
  darc::component_manager mngr;
  iris::glog<iris::Info>("Darc ComponentManager Running");

  // Parse Arguments
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message")
    ("load,l", po::value< std::vector<std::string> >(), "components to load and start");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help"))
  {
    std::cout << desc << std::endl;
    return 1;
  }

  // Find a free port in 5000-5009 range to listen to
  // Connect on all the other ports in the range
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
      catch(darc::network::address_in_use_exception& e)
      {
        std::cout << e.what() << std::endl;
        mngr.connect(url);
      }
    }
    else
    {
      mngr.connect(url);
    }
  }

  // Load Components
  if(vm.count("load"))
  {
    for(StringVecType::const_iterator it = vm["load"].as<StringVecType>().begin();
        it != vm["load"].as<StringVecType>().end();
        it++)
    {
      iris::glog<iris::Info>("Loading Component",
                             "name", iris::arg<std::string>(*it));
      std::string filename = std::string("lib").append(*it).append(".so");
      darc::component_loader::load_component(filename);
      darc::component_ptr component = darc::Registry::instantiateComponent(*it, &mngr);
      assert(component.get());
      component->run();
    }
  }
  else
  {
    std::cout << "No component to load" << std::endl;
  }

  mngr.run_current_thread();
}
