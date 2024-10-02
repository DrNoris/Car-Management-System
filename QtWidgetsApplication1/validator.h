#pragma once
#include "masina.h"
#include <sstream>
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::ostream;

class ValidatorMasini {
public:
    void validate(const Masina& m);
};

class Exception {
    std::vector<string> msgs;
public:
    Exception(const vector<string>& errors) : msgs{ errors } {};
    std::string toString() const {
        std::ostringstream oss;
        for (const auto& msg : msgs) {
            oss << msg << "\n";
        }
        return oss.str();
    }
    friend ostream& operator<<(ostream& out, const Exception& ex);
};

ostream& operator<<(ostream& out, const Exception& ex);
