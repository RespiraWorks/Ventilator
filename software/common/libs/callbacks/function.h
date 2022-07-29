/* Copyright 2020-2022, RespiraWorks

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

// From https://stackoverflow.com/questions/45088723/stdfunction-with-static-allocation-in-c
// Further reading: https://shaharmike.com/cpp/naive-std-function/

#pragma once

#include <algorithm>
#include <cassert>
#include <type_traits>

static constexpr size_t MaxCallbackSize{200};

template <class A>
class naive_function;
template <typename ReturnValue, typename... Args>
class naive_function<ReturnValue(Args...)> {
 public:
  naive_function() {}
  template <typename T>
  naive_function(T t) : set_(true) {
    static_assert(sizeof(CallableT<T>) <= MaxCallbackSize);
    //    static_assert(sizeof(CallableT<T>) <= sizeof(callable_));
    new (_get()) CallableT<T>(t);
  }
  template <typename T>
  naive_function(T *ptr, ReturnValue (T::*t)(Args...)) : set_(true) {
    static_assert(sizeof(CallableT<T>) <= MaxCallbackSize);
    //    static_assert(sizeof(CallableT<T>) <= sizeof(callable_));
    new (_get()) CallableT<T>(ptr, t);
  }
  naive_function(const naive_function &c) : set_(c.set_) {
    if (c.set_) c._get()->Copy(&callable_);
  }

  naive_function &operator=(const naive_function &c) {
    if (this != &c) {
      if (c.set_) {
        set_ = true;
        c._get()->Copy(&callable_);
      } else
        set_ = false;
    }
    return *this;
  }
  ReturnValue operator()(Args... args) const { return _get()->Invoke(args...); }
  ReturnValue operator()(Args... args) { return _get()->Invoke(args...); }

 private:
  class ICallable {
   public:
    virtual ReturnValue Invoke(Args...) = 0;
    virtual void Copy(void *dst) const = 0;
  };
  ICallable *_get() { return ((ICallable *)&callable_); }
  const ICallable *_get() const { return ((const ICallable *)&callable_); }
  template <typename T>
  class CallableT : public ICallable {
   public:
    CallableT(const T &t) : t_(t) {}

    ReturnValue Invoke(Args... args) override { return t_(std::forward<Args>(args)...); }
    void Copy(void *dst) const override { new (dst) CallableT(*this); }

   private:
    T t_;
  };
  template <typename T>
  class CallableT<ReturnValue (T::*)(Args...)> : public ICallable {
   public:
    CallableT(T *ptr, ReturnValue (T::*t)(Args...)) : ptr_(ptr), t_(t) {}

    ReturnValue Invoke(Args... args) override { return (ptr_->*t_)(std::forward<Args>(args)...); }
    void Copy(void *dst) const override { new (dst) CallableT(*this); }

   private:
    T *ptr_ = nullptr;
    ReturnValue (T::*t_)(Args...) = nullptr;
  };

  static constexpr size_t size() {
    auto f = []() -> void {};
    return std::max(sizeof(CallableT<void (*)()>),
                    std::max(sizeof(CallableT<decltype(f)>),
                             sizeof(CallableT<void (CallableT<void (*)()>::*)()>)));
  };

  typedef unsigned char callable_array[MaxCallbackSize];
  //  typedef unsigned char callable_array[size()];
  typename std::aligned_union<0, callable_array, CallableT<void (*)()>,
                              CallableT<void (CallableT<void (*)()>::*)()>>::type callable_;

  bool set_ = false;
};
