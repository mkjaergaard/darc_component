/*
 * Copyright (c) 2012, Prevas A/S
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
 * DARC Thread Manager class
 *
 * \author Morten Kjaergaard
 */

#include <darc/thread_manager.hpp>
#include <boost/make_shared.hpp>
#include <darc/component.hpp>

namespace darc
{

thread_manager::~thread_manager()
{
  beam::glog<beam::Info>("~thread_manager");
  for(ThreadList::iterator it = thread_list_.begin();
      it != thread_list_.end();
      it++)
  {
    it->second.second->stop_work();
    it->second.first->join();
  }
}

void thread_manager::allocate_thread_and_run(component_ptr component)
{
  thread_list_[component->getID()] =
    boost::make_shared<boost::thread>(boost::bind(&component::work, component));
}

void thread_manager::stop_thread(component_ptr component)
{
  assert(thread_list_.count(component->getID()) != 0);
  component->stop_work();
  thread_list_[component->getID()].first->join();
  thread_list_.erase(component->getID());
}

}
