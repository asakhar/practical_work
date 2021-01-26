#include "AffineCipher.hxx"
#include "AffineRecursiveCipher.hxx"
#include "Cipher.hxx"
#include "CryptoAnalyzer.hxx"
#include "SubstitutionCipher.hxx"
#include <iostream>

int main(int argc, char const* argv[])
{
  Alphabet abc{"abcdefghijklmnopqrstuvwxyz"};
  SubstitutionCipher cipher;
  cipher.updateAbc(abc);
  cipher.updateKey({"abcdz", "dbcay", abc});
  std::string enc = cipher.encode("zaabbbccccddddd");

  std::cout << "Simple substitution cipher:\n Result:\n  " << enc << "\n";
  std::cout << " Source\n  " << cipher.decode(enc) << "\n\n" << std::endl;

  AffineCipher afc;
  afc.updateAbc(abc);
  afc.updateKey({{5, 26}, {7, 26}});
  auto encrypted = afc.encode("iamexhausted");
  std::cout << "Affine cipher:\n Result\n  " << encrypted << "\n\n Source:\n  "
            << afc.decode(encrypted) << "\n\n"
            << std::endl;

  AffineRecursiveCipher arc;
  arc.updateKey(AffineRecursiveKey{.key1 = AffineKey{{5, 26}, {7, 26}},
                                   .key2 = AffineKey{{3, 26}, {15, 26}}});
  arc.updateAbc(abc);
  auto encr = arc.encode("iamverytired");
  std::cout << "Affine recursive cipher:\n Result:\n  " << encr
            << "\n\n Source:\n  " << arc.decode(encr) << std::endl;

  Analyzer an{"../ngramms.csv"};

  std::string msg =
// #include "inittext.txt"
#include "testtext.txt"
      ;
  abc.update_from_text(msg);
  afc.updateAbc(abc);
  afc.updateKey({static_cast<int64_t>(abc.size())-1, 5, static_cast<int64_t>(abc.size())});

  auto encrypted1 = afc.encode(msg);
  auto result1 = an.try_analyze_affine(encrypted1, 0.2, 0.3, abc);
  std::cout << result1;
#if 0
  auto res12 = an.count_ngramms(msg, 1);
  an.save_ngramms("../ngramms.csv", res12);
  auto res13 = an.count_ngramms(msg, 2);
  an.save_ngramms("../ngramms.csv", res13);
  auto res14 = an.count_ngramms(msg, 3);
  an.save_ngramms("../ngramms.csv", res14);
#endif

#if 0
  std::string to_decrypt = R"(miRpD ANYtD XY YXDNsa ftDDa rpMr iL rlp NRXJrXJg UJf raNpYprrXJg XJftYrRa. miRpD ANYtD lUY bppJ rlp XJftYrRa'Y YrUJfURf ftDDa rpMr pkpR YXJyp rlp 1500Y, FlpJ UJ tJzJiFJ NRXJrpR riiz U gUsspa iL raNp UJf YyRUDbspf Xr ri DUzp U raNp YNpyXDpJ biiz. Ar lUY YtRkXkpf Jir iJsa LXkp ypJrtRXpY, btr UsYi rlp spUN XJri pspyrRiJXy raNpYprrXJg, RpDUXJXJg pYYpJrXUssa tJylUJgpf. Ar FUY NiNtsURXYpf XJ rlp 1960Y FXrl rlp RpspUYp iL mprRUYpr YlpprY yiJrUXJXJg miRpD ANYtD NUYYUgpY, UJf DiRp RpypJrsa FXrl fpYzriN NtbsXYlXJg YiLrFURp sXzp qsftY cUgpEUzpR XJystfXJg kpRYXiJY iL miRpD ANYtD.)";
  auto decr = an.try_analyze(to_decrypt, 0.2, 0.2, 0.000001, false, true);
#else
  std::string to_decrypt =
#include "testtext.txt"
      ;
  std::cin.get();
  auto decr = an.try_analyze(to_decrypt, 0.26666, 0.4, 0.00068, true, false);
#endif
  std::cout << "\n\n" << decr << "\n\n";

  return 0;
}

// RingElem<int64_t> akey{5, 13};   // 5
// RingElem<int64_t> dkey{-5, 13};  // 8
// RingElem<int64_t> bkey{14, 13};  // 1
// RingElem<int64_t> ckey{-24, 13}; // 2
// auto r1 = akey + dkey;           // 0
// auto r2 = bkey + dkey;           // 9
// auto r3 = akey - dkey;           // 10
// auto r4 = dkey + 15l;            // 10
// auto r5 = ~akey;                 // 8
// auto r6 = ~r2;                   // 3
// auto r7 = ~r3;                   // 4
// auto r8 = ~dkey;                 // 5
// AffineKey keyaff{akey, ckey};
// auto app = keyaff.apply(4l); // 9
// auto res228 = keyaff.cancel(app); // 4