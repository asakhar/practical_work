#ifndef CRYPTOANALYZER_HXX
#define CRYPTOANALYZER_HXX

#include "SubstitutionCipher.hxx"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <unordered_map>
#include <vector>
template <typename Char_t>
struct MyHash
{
  std::size_t operator()(std::pair<Char_t, Char_t> const& s) const noexcept
  {
    auto h1 = std::hash<Char_t>{}(s.first);
    auto h2 = std::hash<Char_t>{}(s.second);
    return h1 ^ (h2 << 1); // or use boost::hash_combine (see Discussion)
  }
};
template <typename Char_t>
struct MyPred
{
  bool operator()(std::pair<Char_t, Char_t> const& s1,
                  std::pair<Char_t, Char_t> const& s2) const noexcept
  {
    return s1.first == s2.first && s1.second == s2.second;
  }
};
template <typename _Key, typename _Tp, typename _Hash = std::hash<_Key>,
          typename _Pred = std::equal_to<_Key>>
class unordered_map_with_default_value
    : public std::unordered_map<_Key, _Tp, _Hash, _Pred>
{
  using _Base = std::unordered_map<_Key, _Tp, _Hash, _Pred>;

public:
  unordered_map_with_default_value(_Tp const& default_value)
      : m_default{default_value}
  {
  }
  _Tp& operator[](_Key const& key)
  {
    if (_Base::find(key) == _Base::end())
      return _Base::operator[](key) = m_default;
    return _Base::operator[](key);
  }
  _Tp& at(_Key const& key)
  {
    if (_Base::find(key) == _Base::end())
      return _Base::at(key) = m_default;
    return _Base::at(key);
  }
  _Tp at(_Key const& key) const
  {
    if (_Base::find(key) == _Base::end())
      return m_default;
    return _Base::at(key);
  }

private:
  _Tp m_default;
};

class Analyzer
{
private:
  using Char_t = char;
  unordered_map_with_default_value<Char_t, double> m_symb2freq{0};
  std::vector<
      unordered_map_with_default_value<std::basic_string<Char_t>, double>>
      m_ngm2freq;
  unordered_map_with_default_value<std::pair<Char_t, Char_t>, double,
                                   MyHash<Char_t>, MyPred<Char_t>>
      m_bigr2freq{0};

public:
  static void dump_double(std::ostream& st, double dbl)
  {
    st.write(reinterpret_cast<char*>(&dbl), sizeof(double));
  }

  static double restore_double(std::istream& st, double& dbl)
  {
    st.read(reinterpret_cast<char*>(&dbl), sizeof(double));
  }

  Analyzer() = default;
  Analyzer(std::string_view freqs_filename, std::string_view bigrs_filename,
           std::string_view ngramms_filename, size_t max_n = 3)
  {
    load_freqs(freqs_filename);
    load_bigrs(bigrs_filename);
    for (auto i = 1ul; i <= max_n; i++)
    {
      m_ngm2freq.emplace_back(0);
      load_ngrs(ngramms_filename, i);
    }
  };
  void load_freqs(std::string_view filename)
  {
    std::ifstream file{filename.data()};
    if (file.fail())
      throw std::runtime_error("File not found.");
    m_symb2freq.clear();
    while (!file.bad() && !file.eof())
    {
      Char_t symb;
      std::basic_string<Char_t> symb1;
      double freq;
      std::getline(file, symb1, ';');
      // file >> symb;
      if (file.bad() || file.eof())
        break;
      assert(symb1.size() == 1 || symb1.size() == 2 && "Input error");
      if (symb1.size() == 1)
        symb = symb1[0];
      else
      {
        if (symb1[1] == 'n')
          symb = '\n';
        else if (symb1[1] == 'c')
          symb = ';';
      }
      // file.get();
      restore_double(file, freq);
      // file >> freq;
      file.get();
      m_symb2freq[symb] = freq;
    }
    file.close();
  }
  void load_bigrs(std::string_view filename)
  {
    std::ifstream file{filename.data()};
    if (file.fail())
      throw std::runtime_error("File not found.");
    m_bigr2freq.clear();
    while (!file.bad() && !file.eof())
    {
      std::basic_string<Char_t> symb1;
      double freq;
      std::getline(file, symb1, ';');
      if (file.bad() || file.eof())
        break;
      assert(symb1.size() == 2 && "Input error");
      // file.get();
      restore_double(file, freq);
      file.get();
      m_bigr2freq[{symb1[0], symb1[1]}] = freq;
    }
    file.close();
  }

  void load_ngrs(std::string_view filename, size_t length)
  {
    std::ifstream file{filename.data()};
    if (file.fail())
      throw std::runtime_error("File not found.");
    // m_bigr2freq.clear();
    while (!file.bad() && !file.eof())
    {
      std::basic_string<Char_t> symb1;
      double freq;
      std::getline(file, symb1, ';');
      if (file.bad() || file.eof())
        break;

      // file.get();
      restore_double(file, freq);
      file.get();
      if (symb1.size() == length)
        m_ngm2freq[length - 1][symb1] = freq;
    }
    file.close();
  }

  auto count_frequencies(std::basic_string_view<Char_t> msg) const
  {
    unordered_map_with_default_value<Char_t, double> resolved_freqs{0};
    for (auto ch : msg)
      resolved_freqs[ch]++;
    for (auto& item : resolved_freqs)
      item.second /= static_cast<double>(msg.size());
    return std::move(resolved_freqs);
  }

  void save_freqs(std::string_view filename,
                  unordered_map_with_default_value<Char_t, double> freqs) const
  {
    std::ofstream file{filename.data()};
    for (auto& item : freqs)
    {
      if (item.first == (Char_t)'\n')
        file << "\\n;";
      else if (item.first == (Char_t)';')
        file << "\\c;";
      else
        file << item.first << ";";
      dump_double(file, item.second);
      file << "\n";
    }
    file.close();
  }

  auto count_bigramms(std::basic_string_view<Char_t> msg) const
  {
    unordered_map_with_default_value<std::pair<Char_t, Char_t>, double,
                                     MyHash<Char_t>, MyPred<Char_t>>
        resolved_freqs{0};
    for (Char_t prev = '\0'; auto ch : msg)
    {
      if (prev != '\0' && prev != '\n' && prev != ';' && prev != ' ' &&
          prev != ',' && ch != '.' && ch != '\n' && ch != ';' && ch != ' ' &&
          ch != ',')
        resolved_freqs[{prev, ch}]++;
      prev = ch;
    }
    for (auto& item : resolved_freqs)
      item.second /= static_cast<double>(msg.size() + 1);
    return std::move(resolved_freqs);
  }

  auto count_ngramms(std::basic_string_view<Char_t> msg, size_t length) const
  {
    unordered_map_with_default_value<std::basic_string<Char_t>, double>
        resolved_freqs{0};

    for (std::basic_stringstream<Char_t> prev; auto ch : msg)
    {
      auto val = prev.tellp() - prev.tellg();
      if (val == length)
      {
        char* copy = new char[length + 1];
        // prev.getline(copy, length + 1);
        prev.read(copy, length);
        copy[length] = 0;
        prev.clear();
        prev << copy + 1;
        resolved_freqs[copy]++;
        delete[] copy;
      }
      if (length == 1 && ch != '\0' && ch != ';')
      {
        prev.put(ch);
      }
      else if (ch != '\0' && ch != ' ' && ch != '\n' && ch != '.' &&
               ch != ',' && ch != ':' && ch != ';' && ch != '\"' && ch != '\'')
      {
        if constexpr (std::is_same_v<Char_t, char>)
          prev.put(static_cast<Char_t>(std::tolower(ch)));
        else
          prev.put(ch);
      }
    }
    for (auto& item : resolved_freqs)
      item.second /= static_cast<double>(msg.size() + 1);
    return std::move(resolved_freqs);
  }

  void save_ngramms(
      std::string_view filename,
      unordered_map_with_default_value<std::basic_string<Char_t>, double> freqs)
      const
  {
    std::ofstream file{filename.data(), std::fstream::app};
    for (auto& item : freqs)
    {
      file << item.first << ";";
      dump_double(file, item.second);
      file << "\n";
    }
    file.close();
  }

  void save_bigramms(
      std::string_view filename,
      unordered_map_with_default_value<std::pair<Char_t, Char_t>, double,
                                       MyHash<Char_t>, MyPred<Char_t>>
          freqs) const
  {
    std::ofstream file{filename.data()};
    for (auto& item : freqs)
    {
      file << item.first.first << item.first.second << ";";
      dump_double(file, item.second);
      file << "\n";
    }

    file.close();
  }
  template <typename _Key, typename... _Args>
  double calc_mean_squared_error(
      unordered_map_with_default_value<_Key, double, _Args...> const& lhs,
      unordered_map_with_default_value<_Key, double, _Args...>& rhs) const
  {
    long double res = 0;
    for (auto& kv : rhs)
      res += std::pow(kv.second - lhs.at(kv.first), 2);

    return static_cast<double>(res / rhs.size());
  }

  template <typename _Key, typename... _Args>
  double calc_mean_abs_error(
      unordered_map_with_default_value<_Key, double, _Args...> const& lhs,
      unordered_map_with_default_value<_Key, double, _Args...>& rhs) const
  {
    long double res = 0;
    for (auto& kv : rhs)
    {
      auto lhsres = lhs.at(kv.first);
      // lhsres *= rhs.size()/lhs.size();
      auto tres   = std::abs(kv.second - lhsres);
      res += tres;
    }

    return static_cast<double>(res / rhs.size());
  }

  std::string try_analyze(std::string_view msg, double alpha, double beta,
                          double precision, bool shuffle = true,
                          bool only_letters = false) const
  {
    std::random_device rd;
    std::mt19937 rand(rd());

    Alphabet abc;
    abc.update_from_text(msg.data());
    auto initial = abc.getLetters();
    auto guessed = initial;
    if (shuffle)
      std::shuffle(guessed.begin(), guessed.end(), rand);

    SubstitutionCipher cipher;
    cipher.updateAbc(abc);
    cipher.updateKey({initial, guessed, abc});

    std::string guess_msg = cipher.decode(msg);
    // auto resolved_let     = count_frequencies(guess_msg);
    // auto resolved_big     = count_bigramms(guess_msg);
    // save_bigramms("../bigrs.csv", resolved_big);
    auto resolved_3gr   = count_ngramms(guess_msg, 3);
    auto resolved_1gr   = count_ngramms(guess_msg, 1);
    auto resolved_2gr   = count_ngramms(guess_msg, 2);
    double mse_letters  = 0;
    double mse_bigramms = 0;
    double mse_3gramms  = 0;
    double mse_1gramms  = 0;
    double mse_2gramms  = 0;
    double total_mse;
#define RECALCULATE()                                                          \
  guess_msg    = cipher.decode(msg);                                           \
  resolved_3gr = count_ngramms(guess_msg, 3);                                  \
  resolved_1gr = count_ngramms(guess_msg, 1);                                  \
  resolved_2gr = count_ngramms(guess_msg, 2);                                  \
  mse_2gramms  = calc_mean_abs_error(m_ngm2freq[1], resolved_2gr);             \
  mse_1gramms  = calc_mean_abs_error(m_ngm2freq[0], resolved_1gr);             \
  mse_3gramms  = calc_mean_abs_error(m_ngm2freq[2], resolved_3gr);             \
  total_mse    = alpha * mse_1gramms + beta * mse_2gramms +                    \
              (1 - alpha - beta) * mse_3gramms;
    // mse_letters  = calc_mean_abs_error(m_symb2freq, resolved_let);               \
  // mse_bigramms = calc_mean_abs_error(m_bigr2freq, resolved_big);               \
  // resolved_let = count_frequencies(guess_msg);                                 \
  // resolved_big = count_bigramms(guess_msg);                                    \

    // 0.4 0.6 0.5
    // 0.266 0.4 0.33333
    // alpha = 0.266
    // beta = 0.4
    RECALCULATE();
    // mse_1gramms = calc_mean_abs_error(m_ngm2freq[0], resolved_1gr);

    while (total_mse > precision)
    {
      auto prev_mse = total_mse;
      auto a = rand() % abc.size(), b = rand() % abc.size();
      while (a == b)
        b = rand() % abc.size();
      char ga = guessed[a], gb = guessed[b];
      if (((ga >= 'a' && ga <= 'z') || (ga >= 'A' && ga <= 'Z')) &&
              ((gb >= 'a' && gb <= 'z') || (gb >= 'A' && gb <= 'Z')) ||
          !only_letters)
      {
        std::swap(guessed[a], guessed[b]);
        cipher.swap_lets(abc.enumerate(guessed[a]), abc.enumerate(guessed[b]));
        RECALCULATE();
      }
      if (total_mse > prev_mse)
      {
        std::swap(guessed[a], guessed[b]);
        cipher.swap_lets(abc.enumerate(guessed[a]), abc.enumerate(guessed[b]));
        total_mse = prev_mse;
        // RECALCULATE();
      }
#ifdef DEBUG
      if (prev_mse != total_mse)
        std::cout << "\n\n\nTotal_mse: " << std::setprecision(9) << total_mse
                  << "    Mse_letters: " << std::setprecision(9) << mse_1gramms
                  << "    Mse_bigramms: " << std::setprecision(9) << mse_2gramms
                  << "    Mse_3gramms: " << std::setprecision(9) << mse_3gramms
                  << "\n\n"
                  << guess_msg << std::endl;
#endif
    }
    return guess_msg;
  }
};

#endif // CRYPTOANALYZER_HXX
