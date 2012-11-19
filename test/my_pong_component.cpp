#include <darc/component.hpp>
#include <beam/glog.hpp>
#include <darc/periodic_timer.hpp>
#include "../include/darc/publisher.h"
#include "../include/darc/subscriber.h"

class my_pong_component : public darc::component
{
  darc::publisher<int> pong_pub_;
  darc::subscriber<int> ping_sub_;

  boost::shared_ptr<int> msg;

  void handler(const boost::shared_ptr<const int>& msg_)
  {
    //beam::glog<beam::Info>("Ping Recv");
    pong_pub_.publish(msg);
  }

public:
  my_pong_component() :
    pong_pub_(this, "pong"),
    ping_sub_(this, "ping", boost::bind(&my_pong_component::handler, this, _1)),
    msg(boost::make_shared<int>(5))
  {
    beam::glog<beam::Info>("my_pong_component created");
  }

};
