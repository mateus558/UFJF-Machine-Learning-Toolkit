#pragma once
#ifndef EXPROPS_HPP_INCLUDED
#define EXPROPS_HPP_INCLUDED

#include <cstddef>
#include <cassert>
#include <memory>
#include <random>

#include "ExprTraits.hpp"

namespace mltk{
    /*! Templates for arithmetic operations implementation.
    \file ExprOps.hpp
    \author Mateus Coutinho Marim
    */
    /*! 
    \class BExprOp
    \author Mateus Coutinho Marim
        
        Base template for binary arithmetic operations implementation.
    */
    template <typename T, typename OP1, typename OP2>
    class  BExprOp {
        protected:
            /// first operand
            typename A_Traits<OP1>::ExprRef op1;
            /// second operand 
            typename A_Traits<OP2>::ExprRef op2; 

        public:
            // constructor initializes references to operands
            BExprOp(OP1 const &a, OP2 const &b): op1(a), op2(b) {}

            virtual T operator[](const size_t& idx) const = 0;
            
            // size is maximum size
            [[nodiscard]] virtual size_t size() const {
                assert((op1.size() == 0) || (op2.size() == 0)
                       || (op1.size() == op2.size()));
                return (op1.size() != 0) ? op1.size() : op2.size();
            }

            virtual ~BExprOp() = default;
    };
    /*! 
    \class A_Add
    \author Mateus Coutinho Marim
        
        Template for addition operation implementation.
    */
    template <typename T, typename OP1, typename OP2>
    class  A_Add: public BExprOp< T, OP1, OP2 > {
        public:
            // constructor initializes references to operands
            A_Add(OP1 const &a, OP2 const &b): BExprOp< T, OP1, OP2 >(a, b) {  }

            // compute sum when value requested
            T operator[] (const size_t& idx) const override { 
                return this->op1[idx] + this->op2[idx];
            }
    };
    /*! 
    \class A_Mult
    \author Mateus Coutinho Marim
        
        Template for multiplication operation implementation.
    */
    template <typename T, typename OP1, typename OP2>
    class A_Mult: public BExprOp< T, OP1, OP2 >{
        public:
            A_Mult(OP1 const &a, OP2 const &b): BExprOp< T, OP1, OP2 >(a, b) {  }
            
            // compute product when value requested
            T operator[] (const size_t& idx) const override { 
                return this->op1[idx] * this->op2[idx]; 
            }
    };
    /*! 
    \class A_Div
    \author Mateus Coutinho Marim
        
        Template for division operation implementation.
    */
    template <typename T, typename OP1, typename OP2>
    class  A_Div: public BExprOp< T, OP1, OP2 >{
        public:
            A_Div(OP1 const &a, OP2 const &b): BExprOp< T, OP1, OP2 >(a, b) {}
            
            // compute product when value requested
            T operator[] (const size_t& idx) const override { 
                return this->op1[idx] / this->op2[idx]; 
            }
    };
    /*! 
    \class A_Sub
    \author Mateus Coutinho Marim
        
        Template for subtraction operation implementation.
    */
    template <typename T, typename OP1, typename OP2>
    class  A_Sub: public BExprOp< T, OP1, OP2 > {
        public:
            A_Sub(OP1 const &a, OP2 const &b): BExprOp< T, OP1, OP2 >(a, b) {  }

            // compute sum when value requested
            T operator[] (const size_t& idx) const override { 
                return this->op1[idx] - this->op2[idx];
            }
    };
    /*! 
    \class UExprOp
    \author Mateus Coutinho Marim
        
        Base template for unary arithmetic operations implementation.
    */
    template <typename T, typename OP >
    class UExprOp{
        protected:
            OP const &op;
        public:
            explicit UExprOp(OP const &op) : op(op) {}

            virtual T operator[](const size_t& idx) const = 0;
    
            [[nodiscard]] virtual std::size_t size() const {
                return op.size();
            }

            virtual ~UExprOp() = default;
    };

    template<typename T, typename OP>
    class F_Exp: public UExprOp<T, OP> {
    public:
        explicit F_Exp(OP const & a): UExprOp<T, OP>(a) {}

        T operator[](const size_t& idx) const override{
            return std::exp(this->op[idx]);
        }
    };

    template<typename T, typename OP>
    class F_Log: public UExprOp<T, OP> {
    public:
        explicit F_Log(OP const & a): UExprOp<T, OP>(a) {}

        T operator[](const size_t& idx) const override{
            return std::log(this->op[idx]);
        }
    };

    template <typename T, typename OP >
    class F_Abs: public UExprOp< T, OP > {
        public:
            explicit F_Abs(OP const& a): UExprOp<T, OP>(a) {}

            T operator[](const size_t& idx) const override {
                return std::fabs(this->op[idx]);
            }
    };

    template <typename T, typename OP >
    class F_Cos: public UExprOp< T, OP > {
    public:
        explicit F_Cos(OP const& a): UExprOp<T, OP>(a) {}

        T operator[](const size_t& idx) const override {
            return std::cos(this->op[idx]);
        }
    };

    template <typename T, typename OP >
    class F_Sin: public UExprOp< T, OP > {
    public:
        explicit F_Sin(OP const& a): UExprOp<T, OP>(a) {}

        T operator[](const size_t& idx) const override {
            return std::sin(this->op[idx]);
        }
    };

    template <typename T, typename POWT, typename OP >
    class F_Pow: public UExprOp< T, OP > {
        private:
            POWT power = POWT();
        public:
            F_Pow(OP const& a, const POWT& power): UExprOp<T, OP>(a), power(power) {}

            T operator[](const size_t& idx) const override {
                return std::pow(this->op[idx], power);
            }
    };

    template <typename T, typename OP1, typename OP2 >
    class  A_Mod: public BExprOp< T, OP1, OP2> {
        public:
            A_Mod(OP1 const& a, OP2 const& b): BExprOp<T, OP1, OP2>(a, b) {}

            T operator[](const size_t& idx) const override {
                return this->op1[idx] % this->op2[idx];
            }
    };

    template <typename T, typename OP1  >
    class  A_Mod< T, OP1, A_Scalar<double> >: public BExprOp< T, OP1, A_Scalar<double> > {
        public:
            A_Mod(OP1 const& a, A_Scalar<double> const& b): BExprOp<T, OP1, A_Scalar<double> >(a, b) {}

            T operator[](const size_t& idx) const override {
                return std::fmod(this->op1[idx], this->op2[idx]);
            }
    };

    template <typename OP1, typename OP2>
    class  A_Mod< double , OP1, OP2 >: public BExprOp< double, OP1, OP2 > {
        public:
            A_Mod(OP1 const& a, OP2 const& b): BExprOp< double, OP1, OP2 >(a, b) {}

            double operator[](const size_t& idx) const override {
                return std::fmod(this->op1[idx], this->op2[idx]);
            }
    };
    

    template<typename T, typename A1, typename A2>
    class  A_Subscript{
        private:
            /// reference to first operand
            A1 const& a1;
            /// reference to second operand
            A2 const& a2;
        public:
            // constructor initializes references to operands
            A_Subscript (A1 const& a, A2 const& b)
            : a1(a), a2(b) {
            }

            // process subscription when value requested
            decltype(auto) operator[] (const size_t& idx) const {
                return a1[a2[idx]];
            }
            
            T& operator[] (const size_t& idx) {
                return a1[a2[idx]];
            }
            
            // size is size of inner array
            [[nodiscard]] std::size_t size() const {
                return a2.size();
            }
    };

    template<typename T, size_t N>
    struct DotProduct{
        static inline T result(T* a, T* b){
            return *a * *b + DotProduct<T, N-1>::result(a+1, b+1);
        }
    };

    template<typename T>
    struct DotProduct<T, 0>{
        static inline T result(T* a, T* b){
            return T{};
        }
    };
}

#endif