#include <darc/component.hpp>
#include <beam/glog.hpp>
#include <darc/periodic_timer.hpp>
#include <darc/procedure_server.hpp>

class my_server_component : public darc::component
{
  darc::procedure_server<int, int, int> server_;

  void call_handler(const darc::ID& call_id, const boost::shared_ptr<const int>& msg)
  {
    server_.result(call_id, msg);
  }

public:
  my_server_component() :
    server_(this, "my_proc",
	    boost::bind(&my_server_component::call_handler, this, _1, _2))
  {
    beam::glog<beam::Info>("my_server_component created");
  }

};
