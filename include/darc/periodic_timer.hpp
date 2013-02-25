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
 * DARC TimerImpl class
 *
 * \author Morten Kjaergaard
 */

#pragma once

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <darc/primitive.hpp>

namespace darc
{

class periodic_timer : public darc::primitive, public boost::asio::deadline_timer
{
public:
  typedef boost::function<void()> CallbackType;

protected:
  typedef boost::shared_ptr<periodic_timer> Ptr;

  CallbackType callback_;

  boost::posix_time::time_duration period_;
  boost::posix_time::ptime expected_deadline_;

public:
  template<typename T>
  periodic_timer(T * owner, void (T::*callback)(), boost::posix_time::time_duration period) :
    darc::primitive(owner),
    boost::asio::deadline_timer(*(owner->io_service()), period),
    callback_(boost::bind(callback, owner)),
    period_(period)
  {
  }

  periodic_timer(darc::owner * owner, CallbackType callback, boost::posix_time::time_duration period, int) :
    darc::primitive(owner),
    boost::asio::deadline_timer(*(owner->io_service()), period),
    callback_(callback),
    period_(period)
  {
  }

  const char * get_type_name()
  {
    return "PeriodicTimers";
  }

  const int get_type_id()
  {
    return 123;
  }

protected:
  void on_start()
  {
    expected_deadline_ = boost::posix_time::microsec_clock::universal_time() + period_;
    expires_from_now( period_ );
    async_wait( boost::bind( &periodic_timer::handler, this, boost::asio::placeholders::error ) );
  }

  void on_stop()
  {
    cancel();
  }

  void on_pause()
  {
  }

  void handler( const boost::system::error_code& error )
  {
    if( error == boost::asio::error::operation_aborted )
    {
      // Happens if we pause it
    }
    else if(error)
    {
//      DARC_WARNING("PeriodicTimer callback gave some error %u", error.value());
    }
    else if( state_ == STOPPED )
    {
      // Just ignore it
    }
    else
    {
      // Todo: system time can actually not be trusted. What about using boost::chrono?
      boost::posix_time::time_duration diff = boost::posix_time::microsec_clock::universal_time() - expected_deadline_;
      expires_from_now( period_ - diff );
      //DARC_INFO("Diff: %s", boost::posix_time::to_simple_string(diff).c_str());
      expected_deadline_ += period_;
      //    std::cout << diff.total_milliseconds() << std::endl;
      async_wait( boost::bind( &periodic_timer::handler, this, boost::asio::placeholders::error ) );

      if(state_ == RUNNING)
      {
        callback_();
      }
    }
  }

  void set_period( boost::posix_time::time_duration new_period )
  {
    period_ = new_period;
  }

};

}
