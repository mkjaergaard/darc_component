#include <darc/darc.hpp>
#include <iris/glog.hpp>

class test_publisher_component : public darc::component
{
  darc::periodic_timer timer_;
  darc::publisher<int> publisher_;

  void timer_callback()
  {
    iris::glog<iris::Info>("publishing");
    publisher_.publish(1);
  }

public:
  test_publisher_component() :
    timer_(this, &test_publisher_component::timer_callback, boost::posix_time::seconds(1)),
    publisher_(this, "topic")
  {
    iris::glog<iris::Info>("test_publisher_component created");
  }

};

DARC_REGISTER_COMPONENT(test_publisher_component)
