#include <cstddef>
#include <cassert>
#include <memory>

#include "ExprTraits.hpp"

template <typename T, typename OP1, typename OP2>
class ExprOp {
    protected:
        typename A_Traits<OP1>::ExprRef op1; // first operand
        typename A_Traits<OP2>::ExprRef op2; // second operand

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

template <typename T, typename OP1, typename OP2>
class A_Mult: public ExprOp< T, OP1, OP2 >{
    public:
        A_Mult(OP1 const &a, OP2 const &b): ExprOp< T, OP1, OP2 >(a, b) {  }
        
        // compute product when value requested
        T operator[] (const size_t &idx) const { 
            return this->op1[idx] * this->op2[idx]; 
        }
};

template <typename T, typename OP1, typename OP2>
class A_Div: public ExprOp< T, OP1, OP2 >{
    public:
        A_Div(OP1 const &a, OP2 const &b): ExprOp< T, OP1, OP2 >(a, b) {}
        
        // compute product when value requested
        T operator[] (const size_t &idx) const { 
            return this->op1[idx] / this->op2[idx]; 
        }
};

template <typename T, typename OP1, typename OP2>
class A_Sub: public ExprOp< T, OP1, OP2 > {
    public:
        A_Sub(OP1 const &a, OP2 const &b): ExprOp< T, OP1, OP2 >(a, b) {  }

        // compute sum when value requested
        T operator[] (const size_t &idx) const { 
            return this->op1[idx] - this->op2[idx];
        }
};
 