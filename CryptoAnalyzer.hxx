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

#include <bits/stdc++.h>

// template <typename Char_t>
// struct MyHash
// {
//   std::size_t operator()(std::pair<Char_t, Char_t> const& s) const noexcept
//   {
//     auto h1 = std::hash<Char_t>{}(s.first);
//     auto h2 = std::hash<Char_t>{}(s.second);
//     return h1 ^ (h2 << 1); // or use boost::hash_combine (see Discussion)
//   }
// };
// template <typename Char_t>
// struct MyPred
// {
//   bool operator()(std::pair<Char_t, Char_t> const& s1,
//                   std::pair<Char_t, Char_t> const& s2) const noexcept
//   {
//     return s1.first == s2.first && s1.second == s2.second;
//   }
// };
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
  _Tp get_default() const { return m_default; }

private:
  _Tp m_default;
};
template <typename Char_t>
class Analyzer
{
private:
  // unordered_map_with_default_value<Char_t, double> m_symb2freq{0};
  std::vector<
      unordered_map_with_default_value<std::basic_string<Char_t>, double>>
      m_ngm2freq;
  // unordered_map_with_default_value<std::pair<Char_t, Char_t>, double,
  //                                  MyHash<Char_t>, MyPred<Char_t>>
  //     m_bigr2freq{0};

public:
  template <typename _Ty>
  static void dump(std::ostream& st, _Ty const& var, size_t count = 1)
  {
    st.write(reinterpret_cast<char const*>(&var), sizeof(_Ty) * count);
  }
  template <typename _Ty>
  static void restore(std::istream& st, _Ty& var, size_t count = 1)
  {
    st.read(reinterpret_cast<char*>(&var), sizeof(_Ty) * count);
  }

  // static void dump_double(std::basic_ostream<Char_t>& st, double dbl)
  // {
  //   st.write(reinterpret_cast<Char_t*>(&dbl), sizeof(double));
  // }
  // static void restore_double(std::basic_istream<Char_t>& st, double& dbl)
  // {
  //   st.read(reinterpret_cast<Char_t*>(&dbl), sizeof(double));
  // }

  Analyzer() = default;
  Analyzer(std::string_view ngramms_filename, size_t max_n = 3)
  {
    for (auto i = 1ul; i <= max_n; i++)
      m_ngm2freq.emplace_back(0);
    load_ngrs(ngramms_filename);
  };

  void load_ngrs(std::string_view filename)
  {
    std::ifstream file{filename.data()};
    if (file.fail())
      throw std::runtime_error("File not found.");
    while (!file.bad() && !file.eof())
    {
      size_t rows = 0;
      restore(file, rows);
      for (size_t i = 0; !file.bad() && !file.eof() && i < rows; i++)
      {
        uint8_t length_cur;
        restore(file, length_cur);
        Char_t tmp[256];
        std::memset((void*)tmp, 0, sizeof(tmp));
        restore(file, *tmp, length_cur);
        if (file.bad() || file.eof())
          break;

        double freq;
        restore(file, freq);
        m_ngm2freq[length_cur - 1][tmp] = freq;
      }
    }
    file.close();
  }

  /*
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
                    unordered_map_with_default_value<Char_t, double> freqs)
    const
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
  */
  auto count_ngramms(std::basic_string_view<Char_t> msg, size_t length,
                     Alphabet<Char_t> const& abc) const
  {
    unordered_map_with_default_value<std::basic_string<Char_t>, double>
        resolved_freqs{0};

    for (std::basic_stringstream<Char_t> prev; auto ch : msg)
    {
      auto val = prev.tellp() - prev.tellg();
      if (val == length)
      {
        Char_t* copy = new Char_t[length + 1];
        prev.read(copy, length);
        copy[length] = 0;
        prev.clear();
        prev << copy + 1;
        resolved_freqs[copy]++;
        delete[] copy;
      }
      if (abc.has(ch))
      {
        if constexpr (std::is_same_v<Char_t, char>)
          prev.put(static_cast<Char_t>(std::tolower(ch)));
        else if constexpr (std::is_same_v<Char_t, wchar_t>)
          prev.put(static_cast<Char_t>(std::towlower(ch)));
        else
          prev.put(ch);
      }
      else
      {
        prev.str(std::basic_string<Char_t>{});
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
    dump(file, freqs.size());
    for (auto& item : freqs)
    {
      dump(file, (int8_t)item.first.size());
      dump(file, *item.first.c_str(), item.first.size());
      dump(file, item.second);
    }
    file.close();
  }

  template <typename _Ty>
  static double calc_mean_squared_error(_Ty const& lhs, _Ty const& rhs)
  {
    long double res = 0;
    for (auto& kv : rhs)
      res += std::pow(kv.second - lhs.at(kv.first), 2);

    return static_cast<double>(res / rhs.size());
  }

  template <typename _Ty>
  static double calc_mean_abs_error(_Ty const& lhs, _Ty const& rhs)
  {
    long double res = 0;
    for (auto& kv : rhs)
    {
      auto lhsres = lhs.at(kv.first);
      auto tres   = std::abs(kv.second - lhsres);
      res += tres;
    }

    return static_cast<double>(res / rhs.size());
  }

  // template <typename _Key, typename... _Args>
  // static double calc_mean_squared_error(
  //     unordered_map_with_default_value<_Key, double, _Args...> const& lhs,
  //     unordered_map_with_default_value<_Key, double, _Args...> const& rhs)
  // {
  //   long double res = 0;
  //   for (auto& kv : rhs)
  //     res += std::pow(kv.second - lhs.at(kv.first), 2);
  //   return static_cast<double>(res / rhs.size());
  // }
  // template <typename _Key, typename... _Args>
  // static double calc_mean_abs_error(
  //     unordered_map_with_default_value<_Key, double, _Args...> const& lhs,
  //     unordered_map_with_default_value<_Key, double, _Args...> const& rhs)
  // {
  //   long double res = 0;
  //   for (auto& kv : rhs)
  //   {
  //     auto lhsres = lhs.at(kv.first);
  //     auto tres   = std::abs(kv.second - lhsres);
  //     res += tres;
  //   }
  //   return static_cast<double>(res / rhs.size());
  // }

  std::basic_string<Char_t> try_analyze(std::basic_string_view<Char_t> msg,
                                        double alpha, double beta,
                                        double precision, bool shuffle = true,
                                        bool only_letters    = false,
                                        Alphabet<Char_t> abc = {}) const
  {
    std::random_device rd;
    std::mt19937 rand(rd());
    if (abc.size() == 0)
      abc.update_from_text(msg.data());
    auto initial = abc.getLetters();
    auto guessed = initial;
    if (shuffle)
      std::shuffle(guessed.begin(), guessed.end(), rand);

    SubstitutionCipher<Char_t> cipher;
    cipher.updateAbc(abc);
    cipher.updateKey({initial, guessed, abc});

    auto guess_msg     = cipher.decode(msg);
    auto resolved_3gr  = count_ngramms(guess_msg, 3, abc);
    auto resolved_1gr  = count_ngramms(guess_msg, 1, abc);
    auto resolved_2gr  = count_ngramms(guess_msg, 2, abc);
    double mse_3gramms = 0;
    double mse_1gramms = 0;
    double mse_2gramms = 0;
    double total_mse;
#define RECALCULATE()                                                          \
  guess_msg    = cipher.decode(msg);                                           \
  resolved_3gr = count_ngramms(guess_msg, 3, abc);                             \
  resolved_1gr = count_ngramms(guess_msg, 1, abc);                             \
  resolved_2gr = count_ngramms(guess_msg, 2, abc);                             \
  mse_2gramms  = calc_mean_abs_error(m_ngm2freq[1], resolved_2gr);             \
  mse_1gramms  = calc_mean_abs_error(m_ngm2freq[0], resolved_1gr);             \
  mse_3gramms  = calc_mean_abs_error(m_ngm2freq[2], resolved_3gr);             \
  total_mse    = alpha * mse_1gramms + beta * mse_2gramms +                    \
              (1 - alpha - beta) * mse_3gramms;

    RECALCULATE();

#if DEBUG
    std::cout << "\n\n\n";
#endif
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
      }
#ifdef DEBUG
      if (prev_mse != total_mse)
        std::cout << "\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r"
                     "\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r"
                     "\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r"
                  << " Total_mse: " << std::setprecision(9) << total_mse
                  << " Mse_letters: " << std::setprecision(9) << mse_1gramms
                  << " Mse_bigramms: " << std::setprecision(9) << mse_2gramms
                  << " Mse_3gramms: " << std::setprecision(9) << mse_3gramms
                  << " Required precision:" << precision << std::flush;
#endif
    }
#if DEBUG
    std::cout << "\n\n\n" << std::flush;
#endif
    return guess_msg;
  }

  std::basic_string<Char_t>
  try_analyze_affine(std::basic_string_view<Char_t> msg, double alpha,
                     double beta, double treshold, Alphabet<Char_t> abc) const
  {
    AffineCipher<Char_t> cipher;
    cipher.updateAbc(abc);
    int64_t alpha_k = 1, beta_k = 0;
    cipher.updateKey(AffineKey{alpha_k, beta_k, abc.size()});

    std::basic_string<Char_t> guess_msg = cipher.decode(msg);
    auto resolved_3gr                   = count_ngramms(guess_msg, 3, abc);
    auto resolved_1gr                   = count_ngramms(guess_msg, 1, abc);
    auto resolved_2gr                   = count_ngramms(guess_msg, 2, abc);
    double mse_3gramms                  = 0;
    double mse_1gramms                  = 0;
    double mse_2gramms                  = 0;
    double total_mse;

    RECALCULATE();
#if DEBUG
    std::wcout << L"\n\n\n";
#endif
    for (; alpha_k < abc.size(); alpha_k++)
    {
      if (std::__gcd(static_cast<size_t>(alpha_k), abc.size()) != 1)
        continue;
      for (beta_k = 0; beta_k < abc.size() && total_mse > treshold; beta_k++)
      {
        auto prev_mse = total_mse;
        auto prev_key = cipher.getKey();
        cipher.updateKey({alpha_k, beta_k, abc.size()});

        RECALCULATE();

#ifdef DEBUG
        std::wcout << L"\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r"
                      L"\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r"
                      L"\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r"
                      L"\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r"
                      L"\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r"
                      L"\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r"
                      L"\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r"
                      L"\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r"
                      L"\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r"
                   << L" Total_mse: " << std::setw(12) << total_mse
                   << L" Min_mse: " << std::setw(12)
                   << prev_mse
                   //  << L" Treshold: " << std::setw(12) << treshold
                   << L" Mse_letters: " << std::setw(12) << mse_1gramms
                   << L" Mse_bigramms: " << std::setw(12) << mse_2gramms
                   << L" Mse_3gramms: " << std::setw(12) << mse_3gramms
                   << L" Alpha:" << alpha_k << L" Beta:" << beta_k << L"  "
                   << std::flush;
#endif
        if (total_mse > prev_mse)
        {
          cipher.updateKey(prev_key);
          total_mse = prev_mse;
        }
      }
    }
#if DEBUG
    std::wcout << L"\n\n\n" << std::flush;
#endif
    return cipher.decode(msg);
  }

  std::basic_string<Char_t>
  try_analyze_raffine(std::basic_string_view<Char_t> msg, double alpha,
                      double beta, double treshold, Alphabet<Char_t> abc) const
  {
    AffineRecursiveCipher<Char_t> cipher;
    cipher.updateAbc(abc);
    int64_t alpha_1 = 7, beta_1 = 0, alpha_2 = 3, beta_2 = 0;
    cipher.updateKey(AffineRecursiveKey{{alpha_1, beta_1, abc.size()},
                                        {alpha_2, beta_2, abc.size()}});

    std::basic_string<Char_t> guess_msg = cipher.decode(msg);
    auto resolved_3gr                   = count_ngramms(guess_msg, 3, abc);
    auto resolved_1gr                   = count_ngramms(guess_msg, 1, abc);
    auto resolved_2gr                   = count_ngramms(guess_msg, 2, abc);
    double mse_3gramms                  = 0;
    double mse_1gramms                  = 0;
    double mse_2gramms                  = 0;
    double total_mse;

    RECALCULATE();
#if DEBUG
    std::wcout << L"\n\n\n";
#endif
    for (; alpha_2 < abc.size(); alpha_2++)
    {
      if (std::__gcd(static_cast<size_t>(alpha_2), abc.size()) != 1)
        continue;
      for (; alpha_1 < abc.size(); alpha_1++)
      {
        if (std::__gcd(static_cast<size_t>(alpha_1), abc.size()) != 1)
          continue;
        for (beta_1 = 0; beta_1 < abc.size(); beta_1++)
        {
          for (beta_2 = 0; beta_2 < abc.size() && total_mse > treshold;
               beta_2++)
          {
            auto prev_mse = total_mse;
            auto prev_key = cipher.getKey();
            cipher.updateKey(
                {{alpha_1, beta_1, abc.size()}, {alpha_2, beta_2, abc.size()}});

            RECALCULATE();

#ifdef DEBUG
            std::wcout
                << L"\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r"
                   L"\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r"
                   L"\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r"
                   L"\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r"
                   L"\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r"
                   L"\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r"
                   L"\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r"
                   L"\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r"
                   L"\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r"
                << L" Total_mse: " <<    std::setw(10) << total_mse
                << L" Min_mse: " <<      std::setw(10) << prev_mse
                << L" Mse_letters: " <<  std::setw(10) << mse_1gramms
                << L" Mse_bigramms: " << std::setw(10) << mse_2gramms
                << L" Mse_3gramms: " <<  std::setw(10) << mse_3gramms
                << L" Alpha1:" << alpha_1 << L" Beta1:" << beta_1 << L"  "
                << L" Alpha2:" << alpha_2 << L" Beta2:" << beta_2 << L"  "
                << std::flush;
#endif
            if (total_mse > prev_mse)
            {
              cipher.updateKey(prev_key);
              total_mse = prev_mse;
            }
          }
        }
      }
    }
#if DEBUG
    std::wcout << L"\n\n\n" << std::flush;
#endif
    return cipher.decode(msg);
  }
};

#endif // CRYPTOANALYZER_HXX
