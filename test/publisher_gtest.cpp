#include <gtest/gtest.h>

#include <darc/component_manager.hpp>
#include <darc/component.hpp>
#include <darc/publisher.h>
#include <darc/subscriber.h>

/// move to common
#include <string>
#include <list>

namespace darc
{
namespace test
{

struct event_record
{
  std::string type;
  std::string data;
  std::string extra;

  static bool is_type(const event_record& record, const std::string& type)
  {
    return record.type == type;
  }
};

class event_list
{
protected:

  typedef std::list<event_record> list_type;
  list_type list_;

public:
  void event_callback(const std::string type, const std::string data, const std::string extra)
  {
    event_record r;
    list_type::iterator record = list_.insert(list_.begin(), r);
    record->type = type;
    record->data = data;
    record->extra = extra;
  }

  void clear()
  {
    list_.clear();
  }

  int count_type(const std::string& type)
  {
    int count = 0;
    for(list_type::iterator it = list_.begin();
        it != list_.end();
        it++)
    {
      if(it->type == type)
      {
        count++;
      }
    }
    return count;
  }

  void remove_type(const std::string& type)
  {
    list_.remove_if(boost::bind<bool>(&event_record::is_type, _1, type));
  }

  bool pop_type(const std::string& type)
  {
    for(list_type::iterator it = list_.begin();
        it != list_.end();
        it++)
    {
      if(it->type == type)
      {
        list_.erase(it);
        return true;
      }
    }
    return false;
  }

  bool is_empty()
  {
    return list_.empty();
  }

};

}
}

///

class component1 : public darc::component
{
protected:
  darc::publisher<std::string> pub_;

public:
  component1() :
    pub_(this, "/mytopic")
  {
  }

  void publish(const std::string& data)
  {
    pub_.publish(boost::make_shared<std::string>(data));
  }

};

class component2 : public darc::component
{
protected:
  darc::subscriber<std::string> sub_;

public:
  darc::test::event_list* event_list_;

protected:
  void callback(const boost::shared_ptr<const std::string>& data)
  {
    event_list_->event_callback("component2_sub", *data, "");
  }

public:
  component2() :
    sub_(this, "/mytopic", boost::bind(&component2::callback, this, _1))
  {
  }

};

class PubSubTest : public testing::Test
{
public:
  darc::component_manager mngr1;
  darc::component_manager mngr2;
  darc::test::event_list event_list1;
  darc::test::event_list event_list2;
  boost::shared_ptr<component1> c1_1;
  boost::shared_ptr<component2> c1_2;
  boost::shared_ptr<component2> c2_2;

  PubSubTest()
  {
    c1_1 = boost::make_shared<component1>();
    c1_1->set_name("component1_1");
    mngr1.attach(c1_1);

    c1_2 = boost::make_shared<component2>();
    c1_2->set_name("component1_2");
    c1_2->event_list_ = &event_list1;
    mngr1.attach(c1_2);

    c2_2= boost::make_shared<component2>();
    c2_2->set_name("component2_2");
    c2_2->event_list_ = &event_list2;
    mngr2.attach(c2_2);
  }
};

TEST_F(PubSubTest, PubSub)
{
  c1_1->run();
  c1_2->run();
  c2_2->run();
  mngr1.run();
  mngr2.run();

  mngr1.accept("zmq+tcp://127.0.0.1:6000");
  mngr1.connect("zmq+tcp://127.0.0.1:6001");
  mngr2.accept("zmq+tcp://127.0.0.1:6001");
  mngr2.connect("zmq+tcp://127.0.0.1:6000");

  sleep(1);

  c1_1->publish("blah");
  sleep(1);
  EXPECT_TRUE(event_list1.pop_type("component2_sub"));
  EXPECT_TRUE(event_list1.is_empty());
  EXPECT_TRUE(event_list2.pop_type("component2_sub"));
  EXPECT_TRUE(event_list2.is_empty());

};
