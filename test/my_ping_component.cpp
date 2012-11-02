#include <darc/component.hpp>
#include <beam/glog.hpp>
#include <darc/periodic_timer.hpp>
#include "../include/darc/publisher.h"
#include "../include/darc/subscriber.h"

class my_ping_component : public darc::Component
{
  darc::PeriodicTimer timer_;
  darc::publisher<int> ping_pub_;
  darc::subscriber<int> pong_sub_;

  boost::shared_ptr<int> msg;

  bool run_;
  uint32_t count_;

  void callback()
  {
    if(run_ == false)
    {
      count_ = 0;
      beam::glog<beam::Info>("Start");
      run_ = true;
      ping_pub_.publish(msg);
    }
    else
    {
      run_ = false;
      beam::glog<beam::Info>("Count", "Count", beam::arg<uint32_t>(count_));
    }
  }

  void handler(const boost::shared_ptr<const int>& msg)
  {
    //beam::glog<beam::Info>("Pong Recv");
    count_++;
    ping_pub_.publish(msg);
  }

public:
  my_ping_component() :
    timer_(this, &my_ping_component::callback, boost::posix_time::seconds(1)),
    ping_pub_(this, "ping"),
    pong_sub_(this, "pong", boost::bind(&my_ping_component::handler, this, _1)),
    run_(false),
    count_(0),
    msg(boost::make_shared<int>(5))
  {
    beam::glog<beam::Info>("my_ping_component created");
  }

};
