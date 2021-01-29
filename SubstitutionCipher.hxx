#ifndef SUBSTITUTIONCIPHER_HXX
#define SUBSTITUTIONCIPHER_HXX
#include "Cipher.hxx"
#include <cassert>
#include <iostream>
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
  template <typename Char_t>
  SubstitutionKey(std::basic_string_view<Char_t> in,
                  std::basic_string_view<Char_t> out,
                  Alphabet<Char_t> const& abc)
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
  int64_t apply(int64_t in) const
  {
    // std::wcout << L"Исходная буква '" << (wchar_t)(L'a' + m_forward.at(in) - 1)
    //            << L"'; результирующая буква '" << (wchar_t)(in + L'a' - 1)
    //            << L"'.\n";
    return m_forward.at(in);
  }
  int64_t cancel(int64_t out) const { 
    
    // std::wcout << L"Исходная буква '" << (wchar_t)(L'a' + m_backward.at(out) - 1)
    //            << L"'; результирующая буква '" << (wchar_t)(out + L'a' - 1)
    //            << L"'.\n";
    return m_backward.at(out); }
};

template <typename Char_t = wchar_t>
class SubstitutionCipher : public Cipher<SubstitutionKey, Char_t>
{
  using _Base = Cipher<SubstitutionKey, Char_t>;

public:
  SubstitutionCipher() = default;
  std::basic_string<Char_t>
  encode(std::basic_string_view<Char_t> msg) const override
  {
    std::basic_stringstream<Char_t> ss;
    for (auto item : msg)
      if (_Base::m_abc.has(item))
        ss.put(
            _Base::m_abc.get(_Base::m_key.apply(_Base::m_abc.enumerate(item))));
      else
        ss.put(item);
    return ss.str();
  };
  void swap_lets(int64_t rhs, int64_t lhs) { _Base::m_key.swap_lets(rhs, lhs); }
  std::basic_string<Char_t>
  decode(std::basic_string_view<Char_t> msg) const override
  {
    std::basic_stringstream<Char_t> ss;
    for (auto item : msg)
      if (_Base::m_abc.has(item))
        ss.put(_Base::m_abc.get(
            _Base::m_key.cancel(_Base::m_abc.enumerate(item))));
      else
        ss.put(item);
    return ss.str();
  };
};

#endif // SUBSTITUTIONCIPHER_HXX
