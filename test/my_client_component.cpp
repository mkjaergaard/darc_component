#include <darc/component.hpp>
#include <iris/glog.hpp>
#include <darc/periodic_timer.hpp>
#include <darc/procedure_client.hpp>

class my_client_component : public darc::component
{
  darc::periodic_timer timer_;
  darc::procedure_client<int, int, int> client_;

  boost::shared_ptr<int> msg;

  bool run_;
  uint32_t count_;

  void callback()
  {
    if(run_ == false)
    {
      count_ = 0;
      iris::glog<iris::Info>("Start");
      run_ = true;
      client_.call(msg);
    }
    else
    {
      run_ = false;
      iris::glog<iris::Info>("Count", "Count", iris::arg<uint32_t>(count_));
    }
  }

  void f_handler(const darc::ID&, const boost::shared_ptr<const int>& msg)
  {
  }

  void r_handler(const darc::ID&, const boost::shared_ptr<const int>& msg)
  {
    count_++;
    client_.call(msg);
  }

public:
  my_client_component() :
    timer_(this, &my_client_component::callback, boost::posix_time::seconds(1)),
    client_(this, "my_proc",
	    boost::bind(&my_client_component::f_handler, this, _1, _2),
	    boost::bind(&my_client_component::r_handler, this, _1, _2)),
    run_(false),
    count_(0),
    msg(boost::make_shared<int>(5))
  {
    iris::glog<iris::Info>("my_client_component created");
  }

};
