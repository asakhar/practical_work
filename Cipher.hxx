#ifndef CIPHER_HXX
#define CIPHER_HXX
#include <concepts>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>

template <typename _Ty>
concept Key = requires(_Ty ty)
{
  {
    ty.apply(0ul)
  }
  ->std::same_as<int64_t>;
  {
    ty.cancel(0ul)
  }
  ->std::same_as<int64_t>;
};

template <typename Char_t>
class Alphabet
{
private:
  std::unordered_map<Char_t, int64_t> m_Let2Pos;
  std::unordered_map<int64_t, Char_t> m_Pos2Let;

public:
  Alphabet() = default;
  Alphabet(std::basic_string_view<Char_t> alphabet, bool is_Exp = false)
  {
    if (is_Exp)
    {
      int64_t pos = 1;
      auto first  = std::find(alphabet.begin(), alphabet.end(), '-');
      while (first != alphabet.end())
      {
        for (auto beg = *(first - 1); beg <= *(first + 1); beg++, pos++)
        {
          m_Let2Pos[beg] = pos;
          m_Pos2Let[pos] = beg;
        }
        first = std::find(first + 1, alphabet.end(), '-');
      }
    }
    else
      update(alphabet);
  }
  void update(std::basic_string_view<Char_t> alphabet)
  {
    m_Let2Pos.clear();
    m_Pos2Let.clear();
    m_Let2Pos.reserve(alphabet.size());
    m_Pos2Let.reserve(alphabet.size());

    for (int64_t pos = 1; auto symb : alphabet)
    {
      m_Let2Pos[symb] = pos;
      m_Pos2Let[pos]  = symb;
      pos++;
    }
  }
  void update_from_text(std::basic_string_view<Char_t> text)
  {
    m_Let2Pos.clear();
    m_Pos2Let.clear();
    for (int64_t pos = 1; auto symb : text)
      if (m_Let2Pos.find(symb) == m_Let2Pos.end())
      {
        m_Let2Pos[symb] = pos;
        m_Pos2Let[pos]  = symb;
        pos++;
      }
  }

  std::basic_string<Char_t> getLetters() const
  {
    std::basic_stringstream<Char_t> ss;
    for (auto& kv : m_Let2Pos)
      ss << kv.first;
    std::basic_string<Char_t> copy = ss.str();
    std::sort(copy.begin(), copy.end());
    return copy;
  }

  bool has(Char_t let) const { return m_Let2Pos.find(let) != m_Let2Pos.end(); }

  bool has(int64_t idx) const { return m_Pos2Let.find(idx) != m_Pos2Let.end(); }

  inline Char_t get(int64_t pos) const { return m_Pos2Let.at(pos); }

  std::basic_string<Char_t> get(std::basic_string_view<int64_t> msg) const
  {
    std::basic_stringstream<Char_t> ss;
    for (auto item : msg)
      ss.put(get(item));
    return ss.str();
  }

  inline int64_t enumerate(Char_t let) const { return m_Let2Pos.at(let); }

  std::basic_string<int64_t> enumerate(std::basic_string_view<Char_t> msg) const
  {
    std::basic_stringstream<int64_t> ss;
    for (auto item : msg)
      ss.put(enumerate(item));
    return ss.str();
  }

  inline size_t size() const { return m_Let2Pos.size(); }
};

template <Key Key_t, typename Char_t = wchar_t>
class Cipher
{
protected:
  Alphabet<Char_t> m_abc;
  Key_t m_key;

public:
  Cipher() = default;
  inline void updateKey(Key_t const& key) { m_key = key; }
  inline void updateKey(Key_t&& key) { m_key = std::forward<Key_t>(key); }

  inline Key_t const& getKey() const { return m_key; }
  inline Alphabet<Char_t> const& getAbc() const { return m_abc; }
  inline void updateAbc(Alphabet<Char_t> const& abc) { m_abc = abc; }
  inline void updateAbc(Alphabet<Char_t>&& abc)
  {
    m_abc = std::forward<Alphabet<Char_t>>(abc);
  }

  virtual std::basic_string<Char_t>
  encode(std::basic_string_view<Char_t> msg) const = 0;
  virtual std::basic_string<Char_t>
  decode(std::basic_string_view<Char_t> msg) const = 0;
  virtual ~Cipher()                                = default;
};

#endif // CIPHER_HXX
