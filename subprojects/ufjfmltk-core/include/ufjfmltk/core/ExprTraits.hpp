#pragma once
#ifndef EXPRTRAITS_HPP_INCLUDED
#define EXPRTRAITS_HPP_INCLUDED

namespace mltk{
    template <typename T> class A_Scalar;

    // primary template
    template <typename T> 
    class  A_Traits{
        public:
        using ExprRef = T const&; // type to refer to is constant reference
    };

    // partial specialization for scalars
    template <typename T>
    class  A_Traits<A_Scalar<T> > {
        public:
            using ExprRef = A_Scalar<T>; // type to refer to is ordinary reference
    };
}

#endif
