#ifndef AFFINECIPHER_HXX
#define AFFINECIPHER_HXX

#include "Cipher.hxx"
#include <cassert>
#include <iostream>
template <std::integral Int>
class RingElem
{
private:
  Int value;
  Int modulo = 1;

public:
  RingElem() = default;
  constexpr RingElem(Int val, Int mod) : value{val % mod}, modulo{mod}
  {
    assert(mod > 0 && "Modulo can't be less than 1.");
    if (value < 0)
      value += modulo;
  };
  constexpr RingElem(RingElem const&) = default;
  constexpr RingElem(RingElem&&)      = default;

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
  constexpr AffineKey(RingElem<int64_t> a, RingElem<int64_t> b) : alpha{a}, beta{b} {};
  constexpr AffineKey(int64_t a, int64_t b, int64_t m) : alpha{a, m}, beta{b, m} {};
  int64_t apply(int64_t in) const
  {

    auto res = static_cast<int64_t>(alpha * (in - 1) + beta) + 1;
    // int64_t inva = (~alpha).getValue();
    // std::wcout << L"\nБуква '" << (wchar_t)(L'a'+in-1) << L"' имеет номер '" << in-1 << L"' => номер результирующего символа будет равен " << alpha.getValue() << L"x" << in-1 << L"+" << beta.getValue() << L" mod " << alpha.getModulo() << L" = " << (res-1) << L". Буква: '" << (wchar_t)(L'a'+res-1) << L"'\n";
    
    return static_cast<int64_t>(alpha * (in - 1) + beta) + 1;
  }
  int64_t cancel(int64_t out) const
  {
    // auto res = static_cast<int64_t>((-beta + (out - 1)) * ~alpha) + 1;
    // int64_t inva = (~alpha).getValue();
    // std::wcout << L"\nБуква '" << (wchar_t)(L'a'+out-1) << L"' имеет номер '" << out-1 << L"' => номер результирующего символа будет равен " << inva << L"x(" << out-1 << L"-" << beta.getValue() << L") mod " << alpha.getModulo() << L" = " << (res) << L". Буква: '" << (wchar_t)(L'a'+res-1) << L"'\n";
    // auto res = static_cast<int64_t>((-beta + (out - 1)) * ~alpha) + 1;
    // std::wcout << L"\n𝝰^-1 = " << alpha.getValue() << L"^-1 = " << (~alpha).getValue() << L"\n" << L"Буква '" << (wchar_t)(L'a'+out-1) << L"' имеет номер '" << out-1 << L"' => номер результирующего символа будет равен " << (~alpha).getValue() << L"x(" << out-1 << L"-" << beta.getValue() << L") mod " << alpha.getModulo() << L" = " << (res-1) << L". Буква: '" << (wchar_t)(L'a'+res-1) << L"'\n";
    return static_cast<int64_t>((-beta + (out - 1)) * ~alpha) + 1;
  }
  int64_t getAlpha() const {
    return alpha.getValue();
  }
  int64_t getBeta() const {
    return beta.getValue();
  }
  friend class AffineRecursiveKey;
};

template<typename Char_t = wchar_t>
class AffineCipher : public Cipher<AffineKey, Char_t>
{
  using _Base = Cipher<AffineKey, Char_t>;
public:
  AffineCipher() = default;
  AffineCipher(Alphabet<Char_t> const& abc, int64_t alpha, int64_t beta) {
    updateAbc(abc);
    updateKey({alpha, beta, abc.size()});
  }
  std::basic_string<Char_t> encode(std::basic_string_view<Char_t> msg) const override
  {
    std::basic_stringstream<Char_t> ss;
    for (auto item : msg)
      if(_Base::m_abc.has(item))
        ss.put(_Base::m_abc.get(_Base::m_key.apply(_Base::m_abc.enumerate(item))));
      else
        ss.put(item);
    return ss.str();
  };
  std::basic_string<Char_t> decode(std::basic_string_view<Char_t> msg) const override
  {
    std::basic_stringstream<Char_t> ss;
    for (auto item : msg)
      if(_Base::m_abc.has(item))
        ss.put(_Base::m_abc.get(_Base::m_key.cancel(_Base::m_abc.enumerate(item))));
      else
        ss.put(item);
    return ss.str();
  };
};

#endif // AFFINECIPHER_HXX
