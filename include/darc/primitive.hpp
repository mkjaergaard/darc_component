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
 * DARC primitive class
 *
 * \author Morten Kjaergaard
 */

#pragma once

#include <map>
#include <darc/id.hpp>

namespace darc
{

class owner;

class primitive
{
  friend class owner;

protected:
  typedef enum {STOPPED, PAUSED, RUNNING} StateType;

  StateType state_;
  ID id_;
  owner * owner_;

  static std::string empty_string_;

  virtual void on_pause() {}
  virtual void on_unpause() {}
  virtual void on_stop() {}
  virtual void on_start() {}
  virtual void on_attach() {};

  virtual void pause()
  {
    if( state_ == RUNNING )
    {
      state_ = PAUSED;
      on_pause();
    }
  }

  virtual void unpause()
  {
    if( state_ == PAUSED )
    {
      state_ = RUNNING;
      on_unpause();
    }
  }

  virtual void stop()
  {
    if( state_ != STOPPED )
    {
      state_ = STOPPED;
      on_stop();
    }
  }

  virtual void start()
  {
    if( state_ == STOPPED )
    {
      state_ = RUNNING;
      on_start();
    }
  }

public:
  primitive(owner * owner);

  virtual ~primitive()
  {}

  virtual const std::string& get_instance_nameme() { return empty_string_; }
  virtual const char * get_type_name() { return ""; }
  virtual const int get_type_id() { return 0; }

  const ID& get_id() const
  {
    return id_;
  }

};

}
