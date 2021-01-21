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

class AffineRecursiveCipher : public Cipher<AffineRecursiveKey>
{
public:
  std::string encode(std::string_view msg) const override
  {
    auto encmsg = m_abc.enumerate(msg);
    std::basic_stringstream<int64_t> ss;
    for (auto ckey = m_key; auto item : encmsg)
    {
      ss.put(ckey.apply(item));
      ckey.next();
    }
    return m_abc.get(ss.str());
  };
  std::string decode(std::string_view msg) const override
  {
    auto encmsg = m_abc.enumerate(msg);
    std::basic_stringstream<int64_t> ss;
    for (auto ckey = m_key; auto item : encmsg)
    {
      ss.put(ckey.cancel(item));
      ckey.next();
    }
    return m_abc.get(ss.str());
  };
};

#endif // AFFINERECURSIVECIPHER_HXX
