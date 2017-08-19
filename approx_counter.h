/*
 *
 * An efficient and scalable approximate counter (currently base two
 * only), based on Morris, 1977.
 *
 * by Emery Berger <http://www.emeryberger.com>
 *
 * To use, replace a global counter (e.g., of type int) with approx_counter<int>.
 *
 * Supports monotonic increment only (you can use ++ and get the value),
 * though you can also reset the counter.
 *
 */


#ifndef APPROX_COUNTER_H_
#define APPROX_COUNTER_H_

#include <atomic>
#include <thread>
#include <random>

/* Portable thread-local macro stuff. */

#ifndef thread_local
# if __STDC_VERSION__ >= 201112 && !defined __STDC_NO_THREADS__
#  define thread_local _Thread_local
# elif defined _WIN32 && ( \
       defined _MSC_VER || \
       defined __ICL || \
       defined __DMC__ || \
       defined __BORLANDC__ )
#  define thread_local __declspec(thread) 
/* note that ICC (linux) and Clang are covered by __GNUC__ */
# elif defined __GNUC__ ||	\
       defined __SUNPRO_C || \
       defined __xlC__
#if !defined(__APPLE__)
#  define thread_local __thread
#endif
# else
#  error "Cannot define thread_local"
# endif
#endif


template <class CounterType>
class approx_counter {
public:
  
  approx_counter()
  {
    reset();
  }

  /// Reset the counter back to zero.
  void reset() {
    _counter = 0;
  }

  /// Get the current value of the counter.
  CounterType operator()() {
    return (((CounterType) 1) << _counter.load()) - 1;
  }

  /// (Approximately) increment the counter.
  approx_counter& operator++() {
    auto currentValue = _counter.load();
    // Increment with probability 2^(-currentValue)
    auto trial = rng() & (((CounterType) 1 << currentValue) - 1);
    if (trial == 0) {
      ++_counter;
    }
    return *this;
  }

private:

  /// Thread-local random number generator.
  static uint64_t rng() {
    static thread_local MWC rng;
    return rng.next();
  }
  
  std::atomic<CounterType> _counter;

  /// Random number generator adapted from Marsaglia.
 
  class MWC {
  private:
    uint64_t _x, _c, _t;

    void init (uint64_t seed1, uint64_t seed2)
    {
      _x = seed1;
      _x <<= 32;
      _x += seed2;
      _c = 123456123456123456ULL;
    }

    inline uint64_t _MWC() {
      _t = (_x << 58) + _c;
      _c = _x >> 6;
      _x += _t;
      _c += (_x < _t);
      return _x;
    }
    
  public:
    
    MWC (uint64_t seed1, uint64_t seed2)
    {
      init (seed1, seed2);
    }
    
    MWC() {
      std::random_device rnd;
      auto seed1 = rnd();
      auto seed2 = rnd();
      init (seed1, seed2);
    }
    
    inline uint64_t next() {
      return _MWC();
    }
  };
  
};

#endif
