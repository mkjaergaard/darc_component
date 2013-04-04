#include <darc/darc.hpp>
#include <iris/glog.hpp>

class test_subscriber_component : public darc::component
{
  darc::subscriber<int> subscriber_;

  void message_handler(const int& msg)
  {
    iris::glog<iris::Info>("Received Message");
  }

public:
  test_subscriber_component() :
    subscriber_(this, "topic", boost::bind(&test_subscriber_component::message_handler, this, _1))
  {
    iris::glog<iris::Info>("test_subscriber_component created");
  }

};

DARC_REGISTER_COMPONENT(test_subscriber_component)
