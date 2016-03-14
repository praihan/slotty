#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>
#include <mutex>

namespace slotty {

namespace detail {

template <typename Policy, typename... Args> class event;
template <typename Policy, typename... Args> class slot;

template <typename Policy, typename... Args> class slot final {

public:
  using policy_type = Policy;
  using callback_type = std::function<void(Args...)>;
  template <typename NewPolicy> struct rebind_policy {
    using type = slot<NewPolicy, Args...>;
  };

public:
  slot() = default;
  slot(slot&&) = default;
  slot& operator=(slot&&) = default;

private:
  slot(const slot&) = delete;
  slot& operator=(const slot&) = delete;

public:
  ~slot() {
    std::lock_guard<typename Policy::mutex_type> lk(this->event_->mutex_);
    auto& l = this->event_->listeners_;
    l.erase(std::remove(l.begin(), l.end(), this), l.end());
  }

private:
  const event<Policy, Args...>* event_;
  callback_type callback_;

  friend class event<Policy, Args...>;
}; // class slot

template <typename Policy, typename... Args>
bool operator==(const detail::slot<Policy, Args...>& a,
                const detail::slot<Policy, Args...>& b) {
  return std::addressof(a) == std::addressof(b);
}

template <typename Policy, typename... Args> class event final {
public:
  using policy_type = Policy;
  using slot_type = typename detail::slot<Policy, Args...>;
  template <typename NewPolicy> struct rebind_policy {
    using type = event<NewPolicy, Args...>;
  };

  event() = default;
  event(event&&) = default;
  ~event() = default;

  template <typename F> void connect(F&& callback, slot_type& slot) const {
    std::lock_guard<typename Policy::mutex_type> lk(mutex_);
    slot.event_ = this;
    slot.callback_ = std::forward<F>(callback);
    this->listeners_.push_back(std::addressof(slot));
  }

  void raise(Args... args) {
    std::lock_guard<typename Policy::mutex_type> lk(mutex_);
    for (auto& l : listeners_)
      l->callback_(std::forward(args)...);
  }

private:
  explicit event(const event&) = delete;
  event& operator=(const event&) = delete;

private:
  mutable typename Policy::template container_type<slot_type*> listeners_;
  mutable typename Policy::mutex_type mutex_;

  friend class detail::slot<Policy, Args...>;
};

template <typename Policy, typename... Args>
bool operator==(const detail::event<Policy, Args...>& a,
                const detail::event<Policy, Args...>& b) {
  return std::addressof(a) == std::addressof(b);
} // class event
} // namespace detail

struct null_basic_mutex {
  void lock() {}
  void unlock() {}
};

struct default_synchronized_policy {
  template <typename T> using container_type = std::vector<T>;
  using mutex_type = std::mutex;
};

struct default_policy {
  template <typename T> using container_type = std::vector<T>;
  using mutex_type = null_basic_mutex;
};

template <typename Policy> struct select_policy {
  using type = Policy;
  template <typename... Args> using slot = detail::slot<Policy, Args...>;
  template <typename... Args> using event = detail::event<Policy, Args...>;
};

template <typename NewPolicy, typename EventOrSlot> struct rebind_policy {
  using type = typename EventOrSlot::template rebind_policy<NewPolicy>::type;
};

template <typename... Args>
using event = detail::event<default_policy, Args...>;

template <typename... Args> using slot = detail::slot<default_policy, Args...>;

template <typename Policy, typename... Args>
detail::slot<Policy, Args...>
make_slot(const detail::event<Policy, Args...>& evt) {
  return detail::slot<Policy, Args...>{};
}

template <typename Policy, typename... Args>
void make_slot(detail::event<Policy, Args...>&& evt) = delete;
} // namespace slotty