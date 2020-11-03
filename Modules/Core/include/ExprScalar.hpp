// class for objects that represent scalars:
template <typename T>
class A_Scalar {
    private:
        T const& value; // value of the scalar
    
    public:
        // Constructor initializes value
        constexpr A_Scalar (T const& v): value (v) {}

        // for index operations, the scalar is the value of each element
        constexpr T const& operator[] (std::size_t ) const { 
            return value;
        }

        // scalars have zero as size
        constexpr std::size_t size () const { 
            return 0; 
        }
};