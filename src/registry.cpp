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
 * DARC Registry class
 *
 * \author Morten Kjaergaard
 */

#include <darc/registry.hpp>
#include <iris/glog.hpp>

namespace darc
{

Registry::Registry()
{
}

Registry * Registry::instance_ = 0;

Registry * Registry::instance()
{
  // todo: better use a mutex
  if( instance_ == 0 )
  {
    instance_ = new Registry();
  }
  return instance_;
}

int Registry::registerComponent(const std::string& component_name, InstantiateComponentMethod method)
{
  Registry * inst = instance();
  inst->component_list_[component_name] = method;
  iris::glog<iris::Info>("Registered Component",
                         "Name", iris::arg<std::string>(component_name));
  return 1;
}

darc::component_ptr Registry::instantiateComponent(const std::string& component_name, component_manager* mngr)
{
  Registry * inst = instance();
  if( inst->component_list_.count(component_name) )
  {
    iris::glog<iris::Info>("Instantiating Component",
                           "Name", iris::arg<std::string>(component_name));
    return inst->component_list_[component_name](component_name, mngr);
  }
  else
  {
    iris::glog<iris::Info>("Trying to instantiate unregistered component",
                           "Name", iris::arg<std::string>(component_name));
    return darc::component_ptr();
  }
}

}
