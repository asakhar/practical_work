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

  Analyzer an{"../freqs.csv", "../bigramms.csv", "../ngramms.csv"};

  // std::ofstream file1("../dumbfile.txt");
  // Analyzer::dump_double(file1, 12.5);
  // file1.close();
  // std::ifstream file2("../dumbfile.txt");
  // std::cout << Analyzer::restore_double(file2)<<std::endl;
  // file2.close();
  std::string msg =
#include "inittext.txt"
      ;
  // msg
//    =
//       R"(Lorem ipsum dolor sit amet, consectetur adipiscing elit.
//       Pellentesque fringilla, nisi maximus tempus euismod, sem mi pretium
//       libero, non porttitor ipsum elit vel ante. Mauris elit enim, vehicula
//       condimentum lectus quis, rutrum dapibus mauris. Nunc sed pharetra enim.
//       Pellentesque rutrum lacus dui, non commodo velit tristique nec. Aliquam
//       mollis, ligula nec rutrum mattis, magna lacus consequat tellus, in
//       sodales ante lectus in sapien. Nunc accumsan massa non orci molestie
//       rhoncus. Nulla efficitur tortor sit amet libero interdum, et tincidunt
//       nibh dignissim. Cras et molestie lacus. Quisque vulputate turpis id
//       eros ultricies ornare. Nunc elementum tristique erat, nec porttitor
//       turpis dignissim vitae.
// Nam quis nisi vel elit iaculis imperdiet. Phasellus tempus, nunc quis porta
// egestas, odio odio malesuada nisl, vitae interdum sapien neque sed ligula.
// Sed non libero nec felis egestas aliquet at ornare metus. Suspendisse sodales
// nisi sed arcu elementum tristique. Curabitur ante dui, mollis ac massa vitae,
// ornare aliquam eros. In pretium egestas nunc, id elementum risus consectetur
// in. Etiam vehicula, metus iaculis feugiat dictum, risus augue scelerisque ex,
// in tincidunt eros elit et elit. Suspendisse et ultricies leo. Sed et sem
// placerat, dictum justo vitae, sagittis libero. Sed condimentum erat eu erat
// finibus volutpat interdum ut odio. Praesent neque massa, vestibulum et
// eleifend in, volutpat non augue. In hac habitasse platea dictumst. Aenean
// vestibulum sit amet urna in interdum. Nullam faucibus diam sed elit ornare
// tincidunt. Phasellus ut pretium felis, vitae dignissim leo. Nullam risus
// ipsum, laoreet eget nulla pharetra, iaculis consectetur massa. Aliquam sapien
// odio, semper in suscipit nec, auctor at tellus. Nullam in orci ex. Aenean
// hendrerit mauris eu mi cursus, id rhoncus arcu euismod. Sed ultrices ligula
// et elit faucibus, id vestibulum nisl varius. Ut malesuada sit amet ipsum eu
// consectetur. Suspendisse a ullamcorper nisi. Donec ac laoreet orci. Donec
// fermentum dignissim massa quis suscipit. Vivamus suscipit porttitor nisi sit
// amet consectetur. Pellentesque non blandit ex, ut facilisis tellus. Nullam
// scelerisque eros nunc, ut viverra elit volutpat eu. Nunc faucibus nulla non
// nisl hendrerit, sit amet dapibus orci vestibulum. Aliquam erat volutpat.
// Class aptent taciti sociosqu ad litora torquent per conubia nostra, per
// inceptos himenaeos. Ut ut leo vel magna tempus laoreet. Nullam mi tortor,
// suscipit non laoreet a, bibendum ac enim. Quisque aliquet faucibus purus
// blandit sodales. Nunc lobortis est at elit malesuada, vel convallis lorem
// vulputate. Nulla fringilla sollicitudin nisi, vel tempor dui convallis id.
// Cras eleifend ullamcorper mauris, ac tempus nisl vestibulum non. Nullam
// tristique luctus dignissim. Nullam accumsan metus in congue convallis.
// Vivamus vehicula eget turpis vel finibus. Maecenas et massa felis.
// Suspendisse tempor arcu turpis, ut congue eros semper sed. Suspendisse
// gravida et arcu ac commodo. Nunc sed diam a dolor pretium consequat. Sed
// scelerisque nibh at feugiat mattis. Fusce eget ligula dui.)";
// // auto res1 = an.count_ngramms(msg, 4);
// // an.save_ngramms("../ngramms.csv", res1);
#if 0
  auto res12 = an.count_ngramms(msg, 1);
  an.save_ngramms("../ngramms.csv", res12);
  auto res13 = an.count_ngramms(msg, 2);
  an.save_ngramms("../ngramms.csv", res13);
  auto res14 = an.count_ngramms(msg, 3);
  an.save_ngramms("../ngramms.csv", res14);
#endif
  // auto freqs1 = an.count_frequencies(msg);
  // an.save_freqs("../freqs.csv", freqs1);
  // auto freqs = an.count_bigramms(msg);
  // an.save_bigramms("../bigramms.csv", freqs);

  abc.update_from_text(msg);
#if 0
  std::string to_decrypt = R"(miRpD ANYtD XY YXDNsa ftDDa rpMr iL rlp NRXJrXJg UJf raNpYprrXJg XJftYrRa. miRpD ANYtD lUY bppJ rlp XJftYrRa'Y YrUJfURf ftDDa rpMr pkpR YXJyp rlp 1500Y, FlpJ UJ tJzJiFJ NRXJrpR riiz U gUsspa iL raNp UJf YyRUDbspf Xr ri DUzp U raNp YNpyXDpJ biiz. Ar lUY YtRkXkpf Jir iJsa LXkp ypJrtRXpY, btr UsYi rlp spUN XJri pspyrRiJXy raNpYprrXJg, RpDUXJXJg pYYpJrXUssa tJylUJgpf. Ar FUY NiNtsURXYpf XJ rlp 1960Y FXrl rlp RpspUYp iL mprRUYpr YlpprY yiJrUXJXJg miRpD ANYtD NUYYUgpY, UJf DiRp RpypJrsa FXrl fpYzriN NtbsXYlXJg YiLrFURp sXzp qsftY cUgpEUzpR XJystfXJg kpRYXiJY iL miRpD ANYtD.)";
  auto decr = an.try_analyze(to_decrypt, 0.2, 0.2, 0.000001, false, true);
#else
  std::string to_decrypt =
#include "testtext.txt"
      ;
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