//
// Created by L. Nagy on 9/13/24.
//

#include "fraction.hpp"

std::ostream &operator<<(std::ostream &out, const Fraction &fp) {
    out << fp.numerator() << "/" << fp.denominator();
    return out;
}

std::ostream &operator<<(std::ostream &out, const FractionPair &fp) {
    out << "(" << fp.first << ", " << fp.second << ")";
    return out;
}