#include "AffineCipher.hxx"
#include "AffineRecursiveCipher.hxx"
#include "Cipher.hxx"
#include "SubstitutionCipher.hxx"
#include <iostream>

int main(int argc, char const* argv[])
{
  Alphabet abc{"abcdefghijklmnopqrstuvwxyz"};
  SubstitutionCipher cipher;
  cipher.updateAbc(abc);
  cipher.updateKey({"abcdz", "dbcay", abc});
  std::string enc = cipher.encode("zaabbccddz");

  std::cout<<"Simple substitution cipher:\n Result:\n  " << enc << "\n";
  std::cout <<" Source\n  "<< cipher.decode(enc) << "\n\n" << std::endl;

  RingElem<int64_t> akey{5, 13};   // 5
  RingElem<int64_t> dkey{-5, 13};  // 8
  RingElem<int64_t> bkey{14, 13};  // 1
  RingElem<int64_t> ckey{-24, 13}; // 2
  auto r1 = akey + dkey;           // 0
  auto r2 = bkey + dkey;           // 9
  auto r3 = akey - dkey;           // 10
  auto r4 = dkey + 15l;            // 10
  auto r5 = ~akey;                 // 8
  auto r6 = ~r2;                   // 3
  auto r7 = ~r3;                   // 4
  auto r8 = ~dkey;                 // 5
  AffineKey keyaff{akey, ckey};
  auto app = keyaff.apply(4l); // 9

  auto res228 = keyaff.cancel(app); // 4

  AffineCipher afc;
  afc.updateAbc(abc);
  afc.updateKey({{5, 26}, {7, 26}});
  auto encrypted = afc.encode("iamexhausted");
  std::cout <<"Affine cipher:\n Result\n  " << encrypted << "\n\n Source:\n  " << afc.decode(encrypted) << "\n\n" << std::endl;

  AffineRecursiveCipher arc;
  arc.updateKey(AffineRecursiveKey{
    .key1 = AffineKey{{5, 26}, {7, 26}},
    .key2 = AffineKey{{3, 26}, {15, 26}}
  });
  arc.updateAbc(abc);
  auto encr = arc.encode("iamverytired");
  std::cout <<"Affine recursive cipher:\n Result:\n  "<< encr << "\n\n Source:\n  " << arc.decode(encr) << std::endl;

  return 0;
}
