/*
 * Copyright (c) 2011, Prevas A/S
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Prevas A/S nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * DARC Component class
 *
 * \author Morten Kjaergaard
 */

#pragma once

#include <boost/asio/io_service.hpp>
#include <boost/scoped_ptr.hpp>
#include <darc/component_fwd.hpp>
#include <darc/owner.hpp>
#include <darc/id.hpp>
#include <darc/component_manager.hpp>

namespace darc
{

class component : public owner
{
  friend class component_manager;

protected:
  std::string name_;
  bool attached_;
  boost::asio::io_service io_service_;
  boost::scoped_ptr<boost::asio::io_service::work> keep_alive_;
  ID id_;

  darc::component_manager * mngr_;

protected:
  component();

public:
  ~component()
  {
    //todo: assert, is running
  }

  void attach_to_manager(darc::component_manager * mngr);
  void set_name(const std::string& instance_name);

  void trigger_on_start();

  virtual void on_start()
  {
  }

public:
  void run();
  void stop();
  void pause();
  void unpause();

  void work();
  void stop_work();

  // impl of darc::owner
  inline boost::asio::io_service * io_service()
  {
    return &io_service_;
  }

  inline darc::component_manager* get_component_manager()
  {
    assert(attached_);
    return mngr_;
  }


  inline const bool& is_attached()
  {
    return attached_;
  }

  // Getters
  inline const std::string get_name() const
  {
    return name_;
  }

  inline const ID& get_id()
  {
    return id_;
  }

  inline const ID& getID()
  {
    return id_;
  }

  // Method to instantiate components
  template<typename T>
  static boost::shared_ptr<T> instantiate(const std::string& instance_name, darc::component_manager* mngr);

};

}

namespace darc
{

template<typename T>
boost::shared_ptr<T> component::instantiate(const std::string& instance_name, darc::component_manager* mngr)
{
  boost::shared_ptr<T> instance(new T());
  instance->set_name(instance_name);
  mngr->attach(instance);
  return instance;
}

}
