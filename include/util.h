#ifndef TUMOR_UTIL_H
#define TUMOR_UTIL_H

#include <fstream>

using ul = unsigned long;
template <class T>
using grid = std::vector<T>;
using bool_wrapper = uint8_t;

// Enum writing to stream.
template<typename T>
std::ostream& operator<<(typename std::enable_if<std::is_enum<T>::value, std::ostream>::type& stream, const T& e)
{
    return stream << static_cast<typename std::underlying_type<T>::type>(e);
}

// Writing vector to stream.
template<class T>
std::ofstream& operator<<(std::ofstream& stream, const grid<T>& grid)
{
    if (!grid.empty()) {
        auto last = grid.end()--;
        for (auto it = grid.begin(); it != last; ++it) {
            stream << *it << ',';
        }
        stream << *last;
    }
    stream << "\n";
    return stream;
}

#endif //TUMOR_UTIL_H
