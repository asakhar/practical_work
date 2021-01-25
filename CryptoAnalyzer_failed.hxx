#ifndef CRYPTOANALYZER_HXX
#define CRYPTOANALYZER_HXX

#include "SubstitutionCipher.hxx"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>
template <typename _Key, typename _Tp>
class unordered_map_with_default_value : public std::unordered_map<_Key, _Tp>
{
public:
  unordered_map_with_default_value(_Tp const& default_value)
      : m_default{default_value}
  {
  }
  _Tp& operator[](_Key const& key)
  {
    if (std::unordered_map<_Key, _Tp>::find(key) ==
        std::unordered_map<_Key, _Tp>::end())
      return std::unordered_map<_Key, _Tp>::operator[](key) = m_default;
    return std::unordered_map<_Key, _Tp>::operator[](key);
  }

private:
  _Tp m_default;
};

class Analyzer
{
private:
  using Char_t = char;
  std::map<Char_t, double> m_symb2freq;

public:
  Analyzer() = default;
  Analyzer(std::string_view filename) { load(filename); };
  void load(std::string_view filename)
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
      file.get();
      file >> freq;
      file.get();
      m_symb2freq[symb] = freq;
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
      if (item.first == (Char_t)'\n')
        file << "\\n;" << item.second << "\n";
      else if (item.first == (Char_t)';')
        file << "\\c;" << item.second << "\n";
      else
        file << item.first << ";" << item.second << "\n";
    file.close();
  }

private:
  void get_predictions_helper(
      std::unordered_map<Char_t, std::vector<Char_t>> const& candidates,
      std::unordered_map<Char_t, std::vector<Char_t>>::const_iterator index,
      std::basic_string<Char_t> const& encrypted,
      std::vector<std::basic_string<Char_t>>& result) const
  {
    if (index == candidates.end())
      return;
    std::vector<std::basic_string<Char_t>> tmp;
    for (auto& cand_for_cur : index->second)
    {
      if (result.size() == 0ul)
      {
        std::basic_stringstream<Char_t> ss;
        for (Char_t symb : encrypted)
          if (symb == index->first)
            ss << cand_for_cur;
          else
            ss << '\0';
        tmp.push_back(ss.str());
      }
      for (auto const& possible_outcome : result)
      {
        tmp.push_back(possible_outcome);
        for (size_t i = 0; Char_t symb : encrypted)
          if (symb == index->first)
          {
            if (tmp.back()[i] != '\0')
              tmp.push_back(possible_outcome);
            tmp.back()[i++] = cand_for_cur;
          }
          else
            i++;
      }
    }
    result = std::move(tmp);
    get_predictions_helper(candidates, ++index, encrypted, result);
  }

public:
  std::vector<std::basic_string<Char_t>>
  get_predictions(std::basic_string<Char_t> encrypted_msg,
                  double precision) const
  {
    std::vector<std::basic_string<Char_t>> result;
    std::unordered_map<Char_t, std::vector<Char_t>> candidates;
    auto resolved_freqs = count_frequencies(encrypted_msg);
    for (auto& item : resolved_freqs)
    {
      for (auto& pred : m_symb2freq)
        if (std::abs(pred.second - item.second) < precision)
          candidates[item.first].push_back(pred.first);
    }
    get_predictions_helper(candidates, candidates.begin(), encrypted_msg,
                           result);
    return result;
  }
};

#endif // CRYPTOANALYZER_HXX
