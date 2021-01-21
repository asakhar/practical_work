#ifndef AFFINECIPHER_HXX
#define AFFINECIPHER_HXX

#include "Cipher.hxx"
#include <cassert>
template <std::integral Int>
class RingElem
{
private:
  Int value;
  Int modulo = 1;

public:
  RingElem() = default;
  RingElem(Int val, Int mod) : value{val % mod}, modulo{mod}
  {
    assert(mod > 0 && "Modulo can't be less than 1.");
    if (value < 0)
      value += modulo;
  };
  RingElem(RingElem const&) = default;
  RingElem(RingElem&&)      = default;

  RingElem& operator=(RingElem const& ot)
  {
    // assert(mod == ot.mod && "Can't assign RingElem with other modulo.");
    value  = ot.value % ot.modulo;
    modulo = ot.modulo;
    return *this;
  }
  RingElem& operator=(RingElem&& ot)
  {
    // assert(mod == ot.mod && "Can't assign RingElem with other modulo.");
    value  = ot.value % ot.modulo;
    modulo = ot.modulo;
    return *this;
  }
  RingElem& operator=(Int const& ot)
  {
    value = ot % modulo;
    return *this;
  }
  RingElem& operator+=(RingElem const& ot)
  {
    assert(modulo == ot.modulo && "Can't operate RingElem with other modulo.");
    value = (value + ot.value) % modulo;
    return *this;
  }
  RingElem& operator+=(Int const& ot)
  {
    value = (value + ot) % modulo;
    if (value < 0)
      value += modulo;
    return *this;
  }
  RingElem& operator-=(RingElem const& ot)
  {
    assert(modulo == ot.modulo && "Can't operate RingElem with other modulo.");
    value = (value - ot.value) % modulo;
    if (value < 0)
      value += modulo;
    return *this;
  }
  RingElem& operator-=(Int const& ot)
  {
    value = (value - ot) % modulo;
    if (value < 0)
      value += modulo;
    return *this;
  }
  RingElem operator+(RingElem const& ot) const
  {
    return RingElem{(value + ot.value) % modulo, modulo};
  }
  RingElem operator+(Int const& ot) const
  {
    return RingElem{(value + ot) % modulo, modulo};
  }
  RingElem operator-(RingElem const& ot) const
  {
    return RingElem{(value - ot.value) % modulo, modulo};
  }
  RingElem operator-(Int const& ot) const
  {
    return RingElem{(value - ot) % modulo, modulo};
  }
  RingElem operator*(RingElem const& ot) const
  {
    return RingElem{(value * ot.value) % modulo, modulo};
  }
  RingElem operator*(Int const& ot) const
  {
    return RingElem{(value * ot) % modulo, modulo};
  }
  RingElem operator~() const
  {
#ifdef NAIVE_INVERSE
    for (Int i = 1; i < modulo; i++)
      if ((*this) * i == RingElem{1, modulo})
        return {i, modulo};
    assert(false && "inverse can't be found.");
#else
    assert(false && "unimplemented.");
#endif
  }
  RingElem operator-() const { return {(modulo - value) % modulo, modulo}; }
  operator Int() const { return value; }
  Int getModulo() const { return modulo; }
  Int getValue() const { return value; }
  bool operator==(RingElem ot) const { return value == ot.value; }
  bool operator!=(RingElem ot) const { return value != ot.value; }
};

class AffineKey
{
private:
  RingElem<int64_t> alpha, beta;

public:
  AffineKey() = default;
  AffineKey(RingElem<int64_t> a, RingElem<int64_t> b) : alpha{a}, beta{b} {};
  AffineKey(int64_t a, int64_t b, int64_t m) : alpha{a, m}, beta{b, m} {};
  int64_t apply(int64_t in) const { return static_cast<int64_t>(alpha * (in - 1) + beta) + 1; }
  int64_t cancel(int64_t out) const
  {
    return static_cast<int64_t>((-beta + (out - 1)) * ~alpha) + 1;
  }
  friend class AffineRecursiveKey;
};

class AffineCipher : public Cipher<AffineKey>
{
public:
  std::string encode(std::string_view msg) const override
  {
    auto encmsg = m_abc.enumerate(msg);
    std::basic_stringstream<int64_t> ss;
    for (auto item : encmsg)
      ss.put(m_key.apply(item));
    return m_abc.get(ss.str());
  };
  std::string decode(std::string_view msg) const override
  {
    auto encmsg = m_abc.enumerate(msg);
    std::basic_stringstream<int64_t> ss;
    for (auto item : encmsg)
      ss.put(m_key.cancel(item));
    return m_abc.get(ss.str());
  };
};

#endif // AFFINECIPHER_HXX
