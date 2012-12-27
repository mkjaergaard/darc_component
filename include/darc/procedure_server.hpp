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
 * DARC Component Wrapper for Procedure Server
 *
 * \author Morten Kjaergaard
 */

#pragma once

#include <darc/primitive.hpp>
#include <darc/owner.hpp>
#include <darc/procedure/server.hpp>

namespace darc
{

template<typename Argument, typename Feedback, typename Result>
class procedure_server : public darc::primitive
{
protected:
  std::string tag_;
  procedure::server<Argument, Feedback, Result> server_;

  typename procedure::server<Argument, Feedback, Result>::call_functor_type call_handler_;

public:
  procedure_server(darc::owner* owner,
		   const std::string& tag,
                   typename procedure::server<Argument, Feedback, Result>::call_functor_type call_handler) :
    darc::primitive(owner),
    tag_(tag),
    call_handler_(call_handler)
  {
  }

  void feedback(const ID& call_id, const boost::shared_ptr<const Feedback>& feedback_msg)
  {
    server_.feedback(call_id, feedback_msg);
  }

  void result(const ID& call_id, const boost::shared_ptr<const Result>& result_msg)
  {
    server_.result(call_id, result_msg);
  }

  void on_attach()
  {
    server_ = darc::procedure::server<Argument, Feedback, Result>(*owner_->io_service(), owner_->get_component_manager()->get_procedure_service());
    server_.set_call_handler(call_handler_);
  }

  void on_start()
  {
    server_.attach(tag_);
  }

  void on_stop()
  {
    server_.detach();
  }

};

}
