cmake_minimum_required(VERSION 3.12)

project(Cifers LANGUAGES CXX)

add_executable(Dialog dial.cpp)
target_compile_definitions(Dialog PUBLIC -DNAIVE_INVERSE)
target_compile_features(Dialog PUBLIC cxx_std_20)
target_precompile_headers(Dialog PUBLIC Cipher.hxx SubstitutionCipher.hxx AffineCipher.hxx)