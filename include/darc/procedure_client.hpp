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
 * DARC Component Wrapper for Procedure Client
 *
 * \author Morten Kjaergaard
 */

#pragma once

#include <darc/primitive.hpp>
#include <darc/owner.hpp>
#include <darc/procedure/client.hpp>

namespace darc
{

template<typename Argument, typename Feedback, typename Result>
class procedure_client : public darc::primitive
{
protected:
  std::string tag_;
  procedure::client<Argument, Feedback, Result> client_;

  typename procedure::client<Argument, Feedback, Result>::feedback_functor_type feedback_handler_;
  typename procedure::client<Argument, Feedback, Result>::result_functor_type result_handler_;

public:
  procedure_client(darc::owner* owner,
		   const std::string& tag,
                   typename procedure::client<Argument, Feedback, Result>::feedback_functor_type feedback_handler,
		   typename procedure::client<Argument, Feedback, Result>::result_functor_type result_handler) :
    darc::primitive(owner),
    tag_(tag),
    feedback_handler_(feedback_handler),
    result_handler_(result_handler)
  {
  }

  void call(const boost::shared_ptr<const Argument>& argument_msg)
  {
    client_.call(argument_msg);
  }

  void on_attach()
  {
    client_ = darc::procedure::client<Argument, Feedback, Result>(*owner_->io_service(), owner_->get_component_manager()->get_procedure_service());
    client_.set_feedback_handler(feedback_handler_);
    client_.set_result_handler(result_handler_);
  }

  void on_start()
  {
    client_.attach(tag_);
  }

  void on_stop()
  {
    client_.detach();
  }

};

}
