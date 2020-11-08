#ifndef EXPROPS_HPP_INCLUDED
#define EXPROPS_HPP_INCLUDED

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
    \class ExprOp
    \author Mateus Coutinho Marim
        
        Base template for arithmetic operations implementation.
    */
    template <typename T, typename OP1, typename OP2>
    class ExprOp {
        protected:
            /// first operand
            typename A_Traits<OP1>::ExprRef op1;
            /// second operand 
            typename A_Traits<OP2>::ExprRef op2; 

        public:
            // constructor initializes references to operands
            ExprOp(OP1 const &a, OP2 const &b): op1(a), op2(b) {}

            virtual T operator[](const size_t &idx) const = 0;
            
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
    class A_Add: public ExprOp< T, OP1, OP2 > {
        public:
            // constructor initializes references to operands
            A_Add(OP1 const &a, OP2 const &b): ExprOp< T, OP1, OP2 >(a, b) {  }

            // compute sum when value requested
            T operator[] (const size_t &idx) const override { 
                return this->op1[idx] + this->op2[idx];
            }
    };
    /*! 
    \class A_Mult
    \author Mateus Coutinho Marim
        
        Template for multiplication operation implementation.
    */
    template <typename T, typename OP1, typename OP2>
    class A_Mult: public ExprOp< T, OP1, OP2 >{
        public:
            A_Mult(OP1 const &a, OP2 const &b): ExprOp< T, OP1, OP2 >(a, b) {  }
            
            // compute product when value requested
            T operator[] (const size_t &idx) const { 
                return this->op1[idx] * this->op2[idx]; 
            }
    };
    /*! 
    \class A_Div
    \author Mateus Coutinho Marim
        
        Template for division operation implementation.
    */
    template <typename T, typename OP1, typename OP2>
    class A_Div: public ExprOp< T, OP1, OP2 >{
        public:
            A_Div(OP1 const &a, OP2 const &b): ExprOp< T, OP1, OP2 >(a, b) {}
            
            // compute product when value requested
            T operator[] (const size_t &idx) const { 
                return this->op1[idx] / this->op2[idx]; 
            }
    };
    /*! 
    \class A_Sub
    \author Mateus Coutinho Marim
        
        Template for subtraction operation implementation.
    */
    template <typename T, typename OP1, typename OP2>
    class A_Sub: public ExprOp< T, OP1, OP2 > {
        public:
            A_Sub(OP1 const &a, OP2 const &b): ExprOp< T, OP1, OP2 >(a, b) {  }

            // compute sum when value requested
            T operator[] (const size_t &idx) const { 
                return this->op1[idx] - this->op2[idx];
            }
    };

    template<typename T, typename A1, typename A2>
    class A_Subscript{
        private:
            // reference to first operand
            A1 const& a1;
            // reference to second operand
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