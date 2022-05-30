#pragma once

namespace mltk{
    /*! 
    \class A_Scalar
    \author Mateus Coutinho Marim
        
        Template for objects that represent scalars.
    */
    template <typename T>
    class  A_Scalar {
        private:
            /// value of the scalar
            T const& value; 
        
        public:
            // Constructor initializes value
            constexpr explicit A_Scalar (T const& v): value (v) {}

            // for index operations, the scalar is the value of each element
            constexpr T const& operator[] (std::size_t ) const {
                return value;
            }

            // scalars have zero as size
            [[nodiscard]] constexpr std::size_t size () const {
                return 0; 
            }
    };
}
