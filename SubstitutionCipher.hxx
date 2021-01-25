#ifndef SUBSTITUTIONCIPHER_HXX
#define SUBSTITUTIONCIPHER_HXX
#include "Cipher.hxx"
#include <cassert>

class SubstitutionKey
{
private:
  std::unordered_map<int64_t, int64_t> m_forward;
  std::unordered_map<int64_t, int64_t> m_backward;

public:
  SubstitutionKey() = default;
  SubstitutionKey(std::basic_string_view<int64_t> in,
                  std::basic_string_view<int64_t> out)
  {
    assert(in.size() == out.size() && "Size mismatch");
    m_forward.reserve(in.size());
    m_backward.reserve(in.size());
    for (auto i = 0ul; i < in.size(); i++)
    {
      m_forward[in[i]]   = out[i];
      m_backward[out[i]] = in[i];
    }
  }
  void swap_lets(int64_t rhs, int64_t lhs)
  {
    std::swap(m_forward[rhs], m_forward[lhs]);
    std::swap(m_backward[m_forward[rhs]], m_backward[m_forward[lhs]]);
  }
  SubstitutionKey(std::string_view in, std::string_view out,
                  Alphabet const& abc)
  {
    assert(in.size() == out.size() && "Size mismatch");
    m_forward.reserve(in.size());
    m_backward.reserve(in.size());
    for (auto i = 0ul; i < in.size(); i++)
    {
      m_forward[abc.enumerate(in[i])]   = abc.enumerate(out[i]);
      m_backward[abc.enumerate(out[i])] = abc.enumerate(in[i]);
    }
  }
  int64_t apply(int64_t in) const { return m_forward.at(in); }
  int64_t cancel(int64_t out) const { return m_backward.at(out); }
};

class SubstitutionCipher : public Cipher<SubstitutionKey>
{
public:
  SubstitutionCipher() = default;
  std::string encode(std::string_view msg) const override
  {
    auto encmsg = m_abc.enumerate(msg);
    std::basic_stringstream<int64_t> ss;
    for (auto item : encmsg)
      ss.put(m_key.apply(item));
    return m_abc.get(ss.str());
  };
  void swap_lets(int64_t rhs, int64_t lhs) { m_key.swap_lets(rhs, lhs); }
  std::string decode(std::string_view msg) const override
  {
    auto encmsg = m_abc.enumerate(msg);
    std::basic_stringstream<int64_t> ss;
    for (auto item : encmsg)
      ss.put(m_key.cancel(item));
    return m_abc.get(ss.str());
  };
};

#endif // SUBSTITUTIONCIPHER_HXX
