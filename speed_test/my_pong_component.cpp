#include <darc/component.hpp>
#include <iris/glog.hpp>
#include <darc/periodic_timer.hpp>
#include <darc/publisher.hpp>
#include <darc/subscriber.hpp>

class my_pong_component : public darc::component
{
  darc::publisher<int> pong_pub_;
  darc::subscriber<int> ping_sub_;

  boost::shared_ptr<int> msg;

  void handler(const int& msg_)
  {
    //iris::glog<iris::Info>("Ping Recv");
    pong_pub_.publish(msg);
  }

public:
  my_pong_component() :
    pong_pub_(this, "pong"),
    ping_sub_(this, "ping", boost::bind(&my_pong_component::handler, this, _1)),
    msg(boost::make_shared<int>(5))
  {
    iris::glog<iris::Info>("my_pong_component created");
  }

};

DARC_REGISTER_COMPONENT(my_pong_component)
