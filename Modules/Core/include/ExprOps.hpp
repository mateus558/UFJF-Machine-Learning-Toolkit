#ifndef EXPROPS_HPP_INCLUDED
#define EXPROPS_HPP_INCLUDED
#pragma once

#include <cstddef>
#include <cassert>
#include <memory>

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
    class BExprOp {
        protected:
            /// first operand
            typename A_Traits<OP1>::ExprRef op1;
            /// second operand 
            typename A_Traits<OP2>::ExprRef op2; 

        public:
            // constructor initializes references to operands
            BExprOp(OP1 const &a, OP2 const &b): op1(a), op2(b) {}

            virtual T operator[](size_t idx) const = 0;
            
            // size is maximum size
            virtual size_t size() const { 
                assert(op1.size() == 0 || op2.size() == 0
                    || op1.size() == op2.size());
                return op1.size() != 0 ? op1.size() : op2.size();
            }
    };
    /*! 
    \class A_Add
    \author Mateus Coutinho Marim
        
        Template for addition operation implementation.
    */
    template <typename T, typename OP1, typename OP2>
    class A_Add: public BExprOp< T, OP1, OP2 > {
        public:
            // constructor initializes references to operands
            A_Add(OP1 const &a, OP2 const &b): BExprOp< T, OP1, OP2 >(a, b) {  }

            // compute sum when value requested
            T operator[] (size_t idx) const override { 
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
            T operator[] (size_t idx) const { 
                return this->op1[idx] * this->op2[idx]; 
            }
    };
    /*! 
    \class A_Div
    \author Mateus Coutinho Marim
        
        Template for division operation implementation.
    */
    template <typename T, typename OP1, typename OP2>
    class A_Div: public BExprOp< T, OP1, OP2 >{
        public:
            A_Div(OP1 const &a, OP2 const &b): BExprOp< T, OP1, OP2 >(a, b) {}
            
            // compute product when value requested
            T operator[] (size_t idx) const { 
                return this->op1[idx] / this->op2[idx]; 
            }
    };
    /*! 
    \class A_Sub
    \author Mateus Coutinho Marim
        
        Template for subtraction operation implementation.
    */
    template <typename T, typename OP1, typename OP2>
    class A_Sub: public BExprOp< T, OP1, OP2 > {
        public:
            A_Sub(OP1 const &a, OP2 const &b): BExprOp< T, OP1, OP2 >(a, b) {  }

            // compute sum when value requested
            T operator[] (size_t idx) const { 
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
            UExprOp(OP const &op) : op(op) {}

            virtual T operator[](size_t idx) const = 0;
    
            virtual std::size_t size() const {
                return op.size();
            }
    };

    template <typename T, typename OP >
    class F_Abs: public UExprOp< T, OP > {
        public:
            F_Abs(OP const& a): UExprOp<T, OP>(a) {}

            T operator[](size_t idx) const override {
                return std::abs(this->op[idx]);
            }
    };

    template <typename T, typename OP >
    class F_Pow: public UExprOp< T, OP > {
        private:
            size_t power = 1;
        public:
            F_Pow(OP const& a, const size_t& power): UExprOp<T, OP>(a), power(power) {}

            T operator[](size_t idx) const override {
                return std::pow(this->op[idx], power);
            }
    };

    template<typename T, typename A1, typename A2>
    class A_Subscript{
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
            decltype(auto) operator[] (std::size_t idx) const {
                return a1[a2[idx]];
            }
            
            T& operator[] (std::size_t idx) {
                return a1[a2[idx]];
            }
            
            // size is size of inner array
            std::size_t size() const {
                return a2.size();
            }
    };
}

#endif