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
 * DARC Node_Impl class
 *
 * \author Morten Kjaergaard
 */

#pragma once

#include <map>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <darc/component_fwd.hpp>
#include <darc/registry.hpp>
#include <darc/peer.hpp>
#include <darc/network/network_manager.hpp>
#include <beam/glog.hpp>
#include <darc/thread_manager.hpp>
#include <darc/distributed_container/container_manager.hpp>
#include <darc/ns_service.hpp>
#include <darc/message_service.hpp>

namespace darc
{

class ComponentManager
{
private:
  boost::asio::io_service io_service_;
  boost::thread node_thread_;

  darc::peer peer_;
  darc::network::network_manager network_mngr_;

  // services
  darc::distributed_container::container_manager container_manager_;
  darc::ns_service ns_service_;
  darc::MessageService message_service_;

  typedef std::map<ID, ComponentPtr> ComponentInstancesList;
  ComponentInstancesList component_instances_;

  ThreadManager thread_manager_;

public:
  ComponentManager() :
    network_mngr_(io_service_, peer_),
    container_manager_(peer_),
    ns_service_(peer_, &container_manager_),
    message_service_(peer_, io_service_, ns_service_)
  {
  }

  MessageService& message_service()
  {
    return message_service_;
  }

protected:
  void work()
  {
    beam::glog<beam::Info>("Running ComponentManager");
    boost::asio::io_service::work keep_alive(io_service_);
    io_service_.run();
  }

public:
  void run()
  {
    node_thread_ = boost::thread( boost::bind(&ComponentManager::work, this) );
  }

  void runCurrentThread()
  {
    work();
  }

  void attach(ComponentPtr component);

  void runComponent(const ID& id)
  {
    assert(component_instances_.count(id) > 0);
    thread_manager_.allocateThreadAndRun(component_instances_[id]);
  }

  void stopComponent(const ID& id)
  {
    assert(component_instances_.count(id) > 0);
    thread_manager_.stopThread(component_instances_[id]);
  }

  void accept(const std::string& url)
  {
    network_mngr_.accept(url);
  }

  void connect(const std::string& url)
  {
    network_mngr_.connect( url);
  }

  const ID& lookupComponentInstance(const std::string& instance_name);

};

}
