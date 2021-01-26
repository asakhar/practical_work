#ifndef AFFINERECURSIVECIPHER_HXX
#define AFFINERECURSIVECIPHER_HXX

#include "AffineCipher.hxx"

struct AffineRecursiveKey
{
  AffineKey key1, key2;
  int64_t apply(int64_t in) const { return key1.apply(in); }
  int64_t cancel(int64_t out) const { return key1.cancel(out); }
  void next()
  {
    AffineKey nexta{key1.alpha * key2.alpha, key1.beta + key2.beta};
    key1 = key2;
    key2 = nexta;
  }
};
template<typename Char_t= wchar_t>
class AffineRecursiveCipher : public Cipher<AffineRecursiveKey, Char_t>
{
  using _Base = Cipher<AffineRecursiveKey, Char_t>;
public:
  std::basic_string<Char_t> encode(std::basic_string_view<Char_t> msg) const override
  {
    std::basic_stringstream<Char_t> ss;
    for (auto ckey = _Base::m_key; auto item : msg)
    {
      if(_Base::m_abc.has(item))
        ss.put(_Base::m_abc.get(ckey.apply(_Base::m_abc.enumerate(item))));
      else
        ss.put(item);
      ckey.next();
    }
    return ss.str();
  };
  std::basic_string<Char_t> decode(std::basic_string_view<Char_t> msg) const override
  {
    std::basic_stringstream<Char_t> ss;
    for (auto ckey = _Base::m_key; auto item : msg)
    {
      if(_Base::m_abc.has(item))
        ss.put(_Base::m_abc.get(ckey.cancel(_Base::m_abc.enumerate(item))));
      else
        ss.put(item);
      ckey.next();
    }
    return ss.str();
  };
};

#endif // AFFINERECURSIVECIPHER_HXX
