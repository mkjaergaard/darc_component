#include <darc/component.hpp>
#include <beam/glog.hpp>
#include <darc/periodic_timer.hpp>
#include "../include/darc/publisher.h"
#include "../include/darc/subscriber.h"

class my_component1 : public darc::component
{
  darc::periodic_timer timer_;
  darc::publisher<int> pub_;
  darc::subscriber<int> sub_;

  void callback()
  {
    beam::glog<beam::Info>("timer callback");
    boost::shared_ptr<int> msg = boost::make_shared<int>(5);
    pub_.publish(msg);
  }

  void handler(const boost::shared_ptr<const int>& msg)
  {
    beam::glog<beam::Info>("msg callback");
  }

public:
  my_component1() :
    timer_(this, &my_component1::callback, boost::posix_time::seconds(1)),
    pub_(this, "mytopic"),
    sub_(this, "mytopic", boost::bind(&my_component1::handler, this, _1))
  {
    beam::glog<beam::Info>("my_component1 created");
  }

};
