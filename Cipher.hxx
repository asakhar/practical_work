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

class Alphabet
{
private:
  std::unordered_map<char, int64_t> m_Let2Pos;
  std::unordered_map<int64_t, char> m_Pos2Let;

public:
  Alphabet() = default;
  Alphabet(std::string_view alphabet) { update(alphabet); }
  void update(std::string_view alphabet)
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

  inline char get(int64_t pos) const { return m_Pos2Let.at(pos); }

  std::string get(std::basic_string_view<int64_t> msg) const
  {
    std::stringstream ss;
    for (auto item : msg)
      ss.put(get(item));
    return ss.str();
  }

  inline int64_t enumerate(char let) const { return m_Let2Pos.at(let); }

  std::basic_string<int64_t> enumerate(std::string_view msg) const
  {
    std::basic_stringstream<int64_t> ss;
    for (auto item : msg)
      ss.put(enumerate(item));
    return ss.str();
  }

  inline size_t size() const { return m_Let2Pos.size(); }
};

template <Key Key_t>
class Cipher
{
protected:
  Alphabet m_abc;
  Key_t m_key;

public:
  Cipher() = default;
  inline void updateKey(Key_t const& key) { m_key = key; }
  inline void updateKey(Key_t&& key) { m_key = std::forward<Key_t>(key); }

  inline Key_t const& getKey() const { return m_key; }
  inline void updateAbc(Alphabet const& abc) { m_abc = abc; }
  inline void updateAbc(Alphabet&& abc) { m_abc = std::forward<Alphabet>(abc); }

  virtual std::string encode(std::string_view msg) const = 0;
  virtual std::string decode(std::string_view msg) const = 0;
  virtual ~Cipher()                                      = default;
};

#endif // CIPHER_HXX
