#include <any>
#include <array>
#include <iostream>
#include <functional>
#include <string>
#include <chrono>
#include <thread>

namespace edges {

template<typename return_type>
struct any_callable {
    any_callable() {}
    template<typename F>
    any_callable(F&& fun) : any_callable(std::function(std::forward<F>(fun))) {}
    template<typename ... Args>
    any_callable(std::function<return_type(Args...)> fun) : m_any(fun) {}
    template<typename ... Args>
    return_type operator()(Args&& ... args) const
    {
        return std::invoke(std::any_cast<std::function<return_type(Args...)>>(m_any), std::forward<Args>(args)...);
    }
    std::any m_any;
};

struct predicate_wrapper : public any_callable<bool> {
    template<typename ... Args>
    //bool operator()(Args&& ... args) const {
    bool operator()(const Args& ... args) const {
        try {
            //return std::invoke(std::any_cast<std::function<bool(Args...)>>(m_any), std::forward<Args>(args)...);
            return std::invoke(std::any_cast<std::function<bool(const Args&...)>>(m_any), args...);
        }
        catch (std::bad_any_cast bac) {
            std::cout << "bad_any_cast ";
            return false;
        }
    }
};

struct timer {
    timer() {
        reset();
    }

    void tick() {
        end = std::chrono::high_resolution_clock::now();
    }
    
    void reset() {
        start = std::chrono::high_resolution_clock::now();
        end = std::chrono::high_resolution_clock::now();
    }

    auto elapsed() const {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
};

enum sequence_policy
{
    weak,
    strict,
};

enum class timeout_policy
{
    none,
    success,
    failure,
};

struct timeout_config
{
    std::chrono::nanoseconds duration_ns{0};
    timeout_policy policy{timeout_policy::none};
};

struct sequence_config
{
    sequence_policy policy {sequence_policy::weak};
    timeout_config timeout;

    bool timeout_active() {
        return timeout.policy != timeout_policy::none;
    }

    bool evaluate_timeout(const timer& timer) {
        switch (timeout.policy) {
        case timeout_policy::failure: {
            return timer.elapsed() > timeout.duration_ns;
        }
        case timeout_policy::success: {
            return timer.elapsed() < timeout.duration_ns;
        }
        case timeout_policy::none:
        default:
            return false;
        }
    }
};

template <std::size_t N> struct sequence {
public:
    template <typename... predicates> constexpr sequence(const sequence_config& config = { sequence_config{} },
        predicates... preds) : _seq{ preds... }, _config{config} {
        static_assert(sizeof...(preds) == N,
            "The number of predicates must match the template parameter N.");
    }

    sequence(const sequence& other) : current(0), _seq(other._seq), _config(other._config) {
    }

    sequence& operator=(const sequence& other) {
        if (this != &other) {
            current = 0;
            _seq = other._seq;
            _config = other._config;
        }
        return *this;
    }

    sequence(sequence&&) = delete;
    sequence& operator=(sequence&&) = delete;

    template<typename ... Args>
    bool try_advance(Args... args) {
        if (current < N) {
            if (_seq[current](std::forward<Args>(args)...)) {
                if (current == 0) {
                    _timer.reset();
                }

                _timer.tick();
                current += 1;

                bool eval = _config.evaluate_timeout(_timer);
                if (_config.timeout_active() && eval) {
                    return evaluate_sequence_with_timeout();
                }
                else {
                    return true;
                }
            }
            else if (_config.policy == sequence_policy::strict) {
                current = 0;
            }
        }
        return false;
    }

    bool is_complete() const {
        return current == N;
    }

    void reset() {
        current = 0;
    }

    float elapsed() const {
        return _timer.elapsed().count() * 0.001f * 0.001f * 0.001f;
    }

private:
    bool evaluate_sequence_with_timeout() {
        if (_config.timeout.policy == timeout_policy::failure) { // timeout occurred before finishing sequence
            reset();
            return false;
        }
        else if (_config.timeout.policy == timeout_policy::success) { // sequence finished before timeout completed
            return true;
        }
        return false;
    }

private:
    mutable size_t current{0};
    std::array<predicate_wrapper, N> _seq;
    timer _timer;

    sequence_config _config;
};

template <typename... predicates> sequence(predicates...) -> sequence<sizeof...(predicates)>;

template <typename... predicates>
constexpr auto make_sequence(predicates&&... preds) {
    return sequence<sizeof...(preds)>(sequence_config{}, std::forward<predicates>(preds)...);
}

template <typename config, typename... predicates>
constexpr auto make_sequence_with_config(const config& conf, predicates&&... preds) {
    return sequence<sizeof...(preds)>(conf, std::forward<predicates>(preds)...);
}
}