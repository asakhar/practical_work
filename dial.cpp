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
  std::string 
  // msg
//    =
//       R"(Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque fringilla, nisi maximus tempus euismod, sem mi pretium libero, non porttitor ipsum elit vel ante. Mauris elit enim, vehicula condimentum lectus quis, rutrum dapibus mauris. Nunc sed pharetra enim. Pellentesque rutrum lacus dui, non commodo velit tristique nec. Aliquam mollis, ligula nec rutrum mattis, magna lacus consequat tellus, in sodales ante lectus in sapien. Nunc accumsan massa non orci molestie rhoncus. Nulla efficitur tortor sit amet libero interdum, et tincidunt nibh dignissim. Cras et molestie lacus. Quisque vulputate turpis id eros ultricies ornare. Nunc elementum tristique erat, nec porttitor turpis dignissim vitae.

// Nam quis nisi vel elit iaculis imperdiet. Phasellus tempus, nunc quis porta egestas, odio odio malesuada nisl, vitae interdum sapien neque sed ligula. Sed non libero nec felis egestas aliquet at ornare metus. Suspendisse sodales nisi sed arcu elementum tristique. Curabitur ante dui, mollis ac massa vitae, ornare aliquam eros. In pretium egestas nunc, id elementum risus consectetur in. Etiam vehicula, metus iaculis feugiat dictum, risus augue scelerisque ex, in tincidunt eros elit et elit. Suspendisse et ultricies leo. Sed et sem placerat, dictum justo vitae, sagittis libero.

// Sed condimentum erat eu erat finibus volutpat interdum ut odio. Praesent neque massa, vestibulum et eleifend in, volutpat non augue. In hac habitasse platea dictumst. Aenean vestibulum sit amet urna in interdum. Nullam faucibus diam sed elit ornare tincidunt. Phasellus ut pretium felis, vitae dignissim leo. Nullam risus ipsum, laoreet eget nulla pharetra, iaculis consectetur massa. Aliquam sapien odio, semper in suscipit nec, auctor at tellus. Nullam in orci ex. Aenean hendrerit mauris eu mi cursus, id rhoncus arcu euismod. Sed ultrices ligula et elit faucibus, id vestibulum nisl varius. Ut malesuada sit amet ipsum eu consectetur. Suspendisse a ullamcorper nisi. Donec ac laoreet orci. Donec fermentum dignissim massa quis suscipit.

// Vivamus suscipit porttitor nisi sit amet consectetur. Pellentesque non blandit ex, ut facilisis tellus. Nullam scelerisque eros nunc, ut viverra elit volutpat eu. Nunc faucibus nulla non nisl hendrerit, sit amet dapibus orci vestibulum. Aliquam erat volutpat. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Ut ut leo vel magna tempus laoreet. Nullam mi tortor, suscipit non laoreet a, bibendum ac enim. Quisque aliquet faucibus purus blandit sodales. Nunc lobortis est at elit malesuada, vel convallis lorem vulputate. Nulla fringilla sollicitudin nisi, vel tempor dui convallis id.

// Cras eleifend ullamcorper mauris, ac tempus nisl vestibulum non. Nullam tristique luctus dignissim. Nullam accumsan metus in congue convallis. Vivamus vehicula eget turpis vel finibus. Maecenas et massa felis. Suspendisse tempor arcu turpis, ut congue eros semper sed. Suspendisse gravida et arcu ac commodo. Nunc sed diam a dolor pretium consequat. Sed scelerisque nibh at feugiat mattis. Fusce eget ligula dui.)";
  

  msg = 
#include "inittext.txt"
  ;
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

//   std::string to_decrypt = R"(miRpD ANYtD XY YXDNsa ftDDa rpMr iL rlp NRXJrXJg UJf raNpYprrXJg XJftYrRa. miRpD ANYtD lUY bppJ rlp XJftYrRa'Y YrUJfURf ftDDa rpMr pkpR YXJyp rlp 1500Y, FlpJ UJ tJzJiFJ NRXJrpR riiz U gUsspa iL raNp UJf YyRUDbspf Xr ri DUzp U raNp YNpyXDpJ biiz. Ar lUY YtRkXkpf Jir iJsa LXkp ypJrtRXpY, btr UsYi rlp spUN XJri pspyrRiJXy raNpYprrXJg, RpDUXJXJg pYYpJrXUssa tJylUJgpf. Ar FUY NiNtsURXYpf XJ rlp 1960Y FXrl rlp RpspUYp iL mprRUYpr YlpprY yiJrUXJXJg miRpD ANYtD NUYYUgpY, UJf DiRp RpypJrsa FXrl fpYzriN NtbsXYlXJg YiLrFURp sXzp qsftY cUgpEUzpR XJystfXJg kpRYXiJY iL miRpD ANYtD.
// )";
  std::string to_decrypt = R"(Nunc in nisi sed sapien molestie bibendum id id erat. Mauris finibus orci vitae tellus tincidunt, nec mollis ligula dapibus. Vivamus congue porta mattis. Nulla rhoncus sit amet magna ut varius. Etiam vel tincidunt orci, quis finibus nulla. Vestibulum dignissim sodales mattis. Suspendisse eu tortor et nulla malesuada accumsan. Cras eu odio eros. Nullam vitae est ipsum. Morbi molestie ex est. Ut placerat urna et ligula laoreet mattis.

Nullam suscipit mi eget mauris vulputate eleifend. Integer elementum velit a elit tempor vehicula. Proin eu massa eget odio consectetur consectetur nec non orci. Ut id risus vitae risus viverra accumsan eget eget augue. Etiam pulvinar consequat elit quis venenatis. Curabitur volutpat ex eu augue porttitor consectetur. Ut suscipit, orci eget elementum molestie, est dolor porta justo, in suscipit diam eros non nisi. Aenean tincidunt pellentesque eros. Nulla et risus vitae leo rutrum posuere a quis elit. Nullam ante ante, molestie non massa ut, laoreet ultrices augue. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus.

Etiam quis pretium velit. Donec auctor nisi orci, eu euismod erat dapibus sed. Duis libero augue, convallis at est at, congue viverra sem. Nulla ut tellus volutpat, consectetur metus gravida, finibus nulla. Maecenas vitae blandit mauris. Maecenas eget odio dignissim massa posuere iaculis. Morbi molestie odio a volutpat posuere. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus quis porttitor sapien. Suspendisse potenti. Vestibulum facilisis porta tellus eget suscipit. Etiam bibendum facilisis condimentum.

Donec vel gravida nulla. Curabitur iaculis maximus consectetur. Vivamus ultrices, massa at congue bibendum, risus ipsum fringilla erat, ut pellentesque augue arcu rhoncus nisl. Cras vulputate mauris ultricies, fermentum magna vitae, egestas lacus. Pellentesque porttitor posuere odio eu accumsan. Quisque arcu justo, dapibus non tortor non, viverra congue augue. Nulla condimentum ex ut nulla tincidunt, nec fermentum purus suscipit. Nullam ac magna nec metus viverra congue.

Mauris et ipsum ut arcu gravida suscipit vitae vel nulla. Mauris lorem sapien, ullamcorper nec aliquet quis, aliquam et magna. Vestibulum tristique sed ante et accumsan. Nunc molestie at turpis non interdum. Etiam imperdiet laoreet magna sit amet pellentesque. Donec placerat eros dolor, eu tempus velit aliquam vel. Vestibulum sit amet malesuada urna. Praesent interdum nunc quis velit auctor, in volutpat magna imperdiet. Aenean ornare luctus ex, non efficitur mi fermentum nec. Ut vitae rhoncus lorem, vel congue orci. Pellentesque vel iaculis eros. Praesent sed sollicitudin lacus. Maecenas vel rutrum velit. Sed enim magna, facilisis nec volutpat eu, tristique non velit.

Proin et urna quis nisi consequat pharetra. Fusce eleifend posuere felis, sed luctus dolor eleifend non. Sed in urna at elit efficitur ultricies. Fusce sit amet arcu id risus auctor hendrerit. Sed ornare enim eu commodo hendrerit. Donec placerat pellentesque purus eget ultricies. Ut et turpis sit amet velit ullamcorper feugiat. Quisque sit amet odio lobortis, eleifend turpis non, fermentum nibh.

Etiam eu semper dui. Curabitur vitae molestie nisi. Nulla ullamcorper ante vitae lectus condimentum efficitur. Praesent egestas scelerisque ante in aliquam. Sed semper orci massa, ac efficitur eros congue vehicula. Praesent in ligula quam. Etiam laoreet et justo non tincidunt. Integer non tellus a diam imperdiet bibendum. Suspendisse eleifend porttitor metus. Vestibulum eleifend mi erat, vitae viverra ligula aliquet a.

Vivamus euismod pellentesque magna, porta tincidunt purus aliquam vel. Donec non ullamcorper turpis. Nam dictum pretium sapien ac tempus. Nulla facilisi. Sed eget arcu a nulla varius venenatis. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Quisque euismod lacus ut erat lobortis ultrices. Cras a molestie elit. Donec a feugiat dui, ut placerat purus. Vestibulum quis ullamcorper ante, nec accumsan orci. Fusce a lorem sed metus volutpat pretium.

Donec porttitor dignissim ipsum, vestibulum vestibulum magna congue et. Sed a vestibulum ipsum. Nam vulputate tristique augue, eget tristique enim vehicula quis. Nunc vulputate molestie pharetra. Sed efficitur vestibulum tortor a condimentum. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Curabitur dictum nec augue ac accumsan. Phasellus a porta felis. Morbi placerat tellus at purus vestibulum scelerisque. Aliquam erat volutpat. Nullam mollis euismod nisi, at dapibus justo lacinia gravida. Donec mattis arcu id diam ornare efficitur. Nunc non elit sed velit hendrerit dapibus. Nulla accumsan id augue et congue. Nunc sit amet odio eget nulla suscipit aliquet eget nec enim. Ut id tortor eu nisl imperdiet aliquet hendrerit imperdiet magna.

Praesent ac facilisis elit, a viverra metus. Maecenas venenatis mauris ut consequat sagittis. In aliquet est a sagittis rutrum. Aenean metus urna, dictum et purus vel, iaculis iaculis ipsum. Etiam non neque id leo aliquet interdum et sed ipsum. Aliquam ultricies non libero sit amet posuere. Sed sagittis pharetra lacus vel dignissim. Nunc a justo a lectus gravida suscipit sagittis ac purus.

Suspendisse cursus mi quis velit vestibulum mattis. Nunc vitae dignissim est, ultricies volutpat sapien. Duis porttitor interdum dapibus. In placerat gravida turpis, id auctor urna mattis sit amet. Donec est risus, aliquet non sem at, aliquam luctus augue. Integer eu ligula consequat, fermentum ante vel, condimentum ligula. Curabitur et ligula urna.

Duis porta sagittis est. Nam ligula ipsum, venenatis eu laoreet non, sollicitudin et est. Cras non finibus leo. Maecenas quis iaculis felis. Donec viverra sapien quis augue luctus, eu tempus enim sagittis. Ut laoreet diam vitae ligula tempor gravida. Nulla molestie diam vel interdum tincidunt. Vivamus sagittis pulvinar ante dapibus porttitor. Praesent a tristique libero. Aliquam id dui eget mi auctor cursus a ac erat. Etiam sollicitudin ipsum ac rutrum bibendum.

Donec id massa consequat, hendrerit quam accumsan, lacinia diam. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Ut in nibh ac ipsum accumsan eleifend in eget est. Maecenas vitae ligula auctor, sodales massa vel, maximus urna. Mauris iaculis ligula vitae tincidunt rutrum. Maecenas malesuada, erat sed elementum ultricies, ex lacus molestie risus, ac sollicitudin eros massa nec odio. Etiam et arcu felis. Cras molestie congue malesuada. Nulla lorem sem, ultricies in lectus et, elementum varius purus. Ut vulputate nibh mauris, elementum varius nulla pharetra ac. Integer mauris erat, sagittis ut consectetur eu, mollis id massa.

Morbi sodales cursus quam, at commodo eros dignissim id. Fusce vehicula hendrerit bibendum. Fusce sodales magna id eros tempus, sit amet pellentesque diam tempor. Phasellus gravida lorem vitae purus elementum, a cursus nibh lobortis. In venenatis ornare justo eget sollicitudin. Vivamus rhoncus arcu vitae dolor dignissim, eget dignissim lorem vulputate. Fusce metus augue, imperdiet placerat egestas sed, dictum sed lacus. Aliquam lobortis lectus vitae augue eleifend blandit. Nunc eros lectus, facilisis ut semper eget, maximus in ligula. Nulla vel fermentum lorem, id rutrum erat. Praesent cursus arcu ac gravida scelerisque. Quisque blandit tincidunt nulla et pellentesque. Suspendisse vestibulum porta leo.

Phasellus ultricies mattis turpis, id tempor odio elementum viverra. Aliquam erat volutpat. Curabitur ullamcorper at odio sed semper. Sed lobortis lobortis diam quis elementum. Phasellus ornare nibh at enim vehicula, id suscipit libero ultricies. Vestibulum et suscipit neque. Aliquam cursus eleifend ornare.)";

  // auto ret = an.get_predictions(msg, 0.000000001);
  // std::cout << "\n\n";
  // for (auto& item : ret)
  //   std::cout << "\n" << item;

  auto decr = an.try_analyze(msg, 0.4, 0.5, 0.000001);
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