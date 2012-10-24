#include <darc/component.hpp>
#include <beam/glog.hpp>
#include <darc/periodic_timer.hpp>

class my_component1 : public darc::Component
{
  darc::timer::PeriodicTimer timer_;

  void callback()
  {
    beam::glog<beam::Info>("timer callback");
  }

public:
  my_component1() :
    timer_(this, &my_component1::callback, boost::posix_time::seconds(1))
  {
    beam::glog<beam::Info>("my_component1 created");
  }

};
