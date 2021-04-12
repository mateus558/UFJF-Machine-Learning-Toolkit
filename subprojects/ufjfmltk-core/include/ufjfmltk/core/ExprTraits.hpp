#pragma once
#ifndef EXPRTRAITS_HPP_INCLUDED
#define EXPRTRAITS_HPP_INCLUDED
#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_CORE
    #define DLLPointOps __declspec(dllexport)
  #else
    #define DLLPointOps __declspec(dllimport)
  #endif
#else
  #ifdef BUILDING_CORE
      #define DLLPointOps __attribute__ ((visibility ("default")))
  #else
      #define DLLPointOps
  #endif
#endif

namespace mltk{
    template <typename T> class A_Scalar;

    // primary template
    template <typename T> 
    class DLLPointOps A_Traits{
        public:
        using ExprRef = T const&; // type to refer to is constant reference
    };

    // partial specialization for scalars
    template <typename T>
    class DLLPointOps A_Traits<A_Scalar<T> > {
        public:
            using ExprRef = A_Scalar<T>; // type to refer to is ordinary reference
    };
}

#endif
