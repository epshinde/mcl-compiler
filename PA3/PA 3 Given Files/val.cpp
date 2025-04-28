#include <iostream>
#include <string>
#include <vector> 
#include "val.h"

using namespace std;

Value Value::operator+(const Value& op) const {
    ValType leftOperandType = this->GetType();
    ValType rightOperandType = op.GetType();
    switch (leftOperandType) {
        case VINT:
            switch(rightOperandType) {
                case VINT:
                    return Value(this->GetInt() + op.GetInt());
                case VREAL:
                    return Value(this->GetInt() + op.GetReal());
                case VCHAR:
                    return Value(this->GetInt() + op.GetChar());
                default:
                    return Value();
            }
        case VREAL:
            switch(rightOperandType) {
                case VINT:
                    return Value(this->GetReal() + op.GetInt());
                case VCHAR:
                    return Value(this->GetReal() + op.GetChar());
                case VREAL:
                    return Value(this->GetReal() + op.GetReal());
                default:
                    return Value();
            }
        case VSTRING:
            switch(rightOperandType) {
                case VSTRING:
                    return Value(this->GetString() + op.GetString());
                case VCHAR:
                    return Value(this->GetString() + op.GetChar());
                default:
                    return Value();
            }
        case VCHAR:
            switch(rightOperandType) {
                case VSTRING:
                    return Value(this->GetChar() + op.GetString());
                case VCHAR:
                    return Value(this->GetChar() + op.GetChar());
                case VINT:
                    return Value(this->GetChar() + op.GetInt());
                case VREAL:
                    return Value(this->GetChar() + op.GetReal());
                default:
                    return Value();
            }
        default:
            return Value();
    }

}

// numeric overloaded subtract op from this
Value Value::operator-(const Value& op) const {
    ValType leftOperandType = this->GetType();
    ValType rightOperandType = op.GetType();
    switch (leftOperandType) {
        case VINT:
            switch(rightOperandType) {
                case VINT:
                    return Value(this->GetInt() - op.GetInt());
                case VREAL:
                    return Value(this->GetInt() - op.GetReal());
                case VCHAR:
                    return Value(this->GetInt() - op.GetChar());
                default:
                    return Value();
            }
        case VREAL:
            switch(rightOperandType) {
                case VINT:
                    return Value(this->GetReal() - op.GetInt());
                case VCHAR:
                    return Value(this->GetReal() - op.GetChar());
                case VREAL:
                    return Value(this->GetReal() - op.GetReal());
                default:
                    return Value();
            }
        case VCHAR:
            switch(rightOperandType) {
                case VCHAR:
                    return Value(this->GetChar() - op.GetChar());
                case VINT:
                    return Value(this->GetChar() - op.GetInt());
                case VREAL:
                    return Value(this->GetChar() - op.GetReal());
                default:
                    return Value();
            }
        default:
            return Value();
    }
}

// numeric overloaded multiply this by op
Value Value::operator*(const Value& op) const {
    ValType leftOperandType = this->GetType();
    ValType rightOperandType = op.GetType();
    switch (leftOperandType) {
        case VINT:
            switch(rightOperandType) {
                case VINT:
                    return Value(this->GetInt() * op.GetInt());
                case VREAL:
                    return Value(this->GetInt() * op.GetReal());
                case VCHAR:
                    return Value(this->GetInt() * op.GetChar());
                default:
                    return Value();
            }
        case VREAL:
            switch(rightOperandType) {
                case VINT:
                    return Value(this->GetReal() * op.GetInt());
                case VREAL:
                    return Value(this->GetReal() * op.GetReal());
                default:
                    return Value();
            }
        case VCHAR:
            switch(rightOperandType) {
                case VCHAR:
                    return Value(this->GetChar() * op.GetChar());
                case VINT:
                    return Value(this->GetChar() * op.GetInt());
                default:
                    return Value();
            }
        default:
            return Value();
    }
};

// numeric overloaded divide this by oper
Value Value::operator/(const Value& op) const {
    ValType leftOperandType = this->GetType();
    ValType rightOperandType = op.GetType();
    switch (leftOperandType) {
        case VINT:
            switch(rightOperandType) {
                case VINT:
                    return Value(this->GetInt() / op.GetInt());
                case VREAL:
                    return Value(this->GetInt() / op.GetReal());
                case VCHAR:
                    return Value(this->GetInt() / op.GetChar());
                default:
                    return Value();
            }
        case VREAL:
            switch(rightOperandType) {
                case VINT:
                    return Value(this->GetReal() / op.GetInt());
                case VREAL:
                    return Value(this->GetReal() / op.GetReal());
                default:
                    return Value();
            }
        case VCHAR:
            switch(rightOperandType) {
                case VCHAR:
                    return Value(this->GetChar() / op.GetChar());
                case VINT:
                    return Value(this->GetChar() / op.GetInt());
                default:
                    return Value();
            }
        default:
            return Value();
    }
}

//numeric overloaded Remainder this by op
Value Value::operator%(const Value & op) const {
    ValType leftOperandType = this->GetType();
    ValType rightOperandType = op.GetType();
    switch (leftOperandType) {
        case VINT:
            switch(rightOperandType) {
                case VINT:
                    return Value(this->GetInt() % op.GetInt());
                case VCHAR:
                    return Value(this->GetInt() % op.GetChar());
                default:
                    return Value();
            }
        case VCHAR:
            switch(rightOperandType) {
                case VCHAR:
                    return Value(this->GetChar() % op.GetChar());
                case VINT:
                    return Value(this->GetChar() % op.GetInt());
                default:
                    return Value();
            }
        default:
            return Value();
    }
}

//overloaded greater than operator of this with op
Value Value::operator>(const Value &op) const {
    ValType leftOperandType = this->GetType();
    ValType rightOperandType = op.GetType();
    switch (leftOperandType) {
        case VINT:
            switch(rightOperandType) {
                case VINT:
                    return Value(this->GetInt() > op.GetInt());
                case VREAL:
                    return Value(this->GetInt() > op.GetReal());
                case VCHAR:
                    return Value(this->GetInt() > op.GetChar());
                default:
                    return Value();
            }
        case VREAL:
            switch(rightOperandType) {
                case VINT:
                    return Value(this->GetReal() > op.GetInt());
                case VREAL:
                    return Value(this->GetReal() > op.GetReal());
                default:
                    return Value();
            }
        case VCHAR:
            switch(rightOperandType) {
                case VCHAR:
                    return Value(this->GetChar() > op.GetChar());
                case VINT:
                    return Value(this->GetChar() > op.GetInt());
                default:
                    return Value();
            }
        case VBOOL:
            switch(rightOperandType) {
                case VBOOL:
                    return Value(this->GetBool() > op.GetBool());
                default:
                    return Value();
            }
        case VSTRING:
            switch(rightOperandType) {
                case VSTRING:
                    return Value(this->GetString() > op.GetString());
                default:
                    return Value();
            }
        default:
            return Value();
    }
}

//overloaded less than operator of this with op
Value Value::operator<(const Value& op) const {
    ValType leftOperandType = this->GetType();
    ValType rightOperandType = op.GetType();
    switch (leftOperandType) {
        case VINT:
            switch(rightOperandType) {
                case VINT:
                    return Value(this->GetInt() < op.GetInt());
                case VREAL:
                    return Value(this->GetInt() < op.GetReal());
                case VCHAR:
                    return Value(this->GetInt() < op.GetChar());
                default:
                    return Value();
            }
        case VREAL:
            switch(rightOperandType) {
                case VINT:
                    return Value(this->GetReal() < op.GetInt());
                case VREAL:
                    return Value(this->GetReal() < op.GetReal());
                default:
                    return Value();
            }
        case VCHAR:
            switch(rightOperandType) {
                case VCHAR:
                    return Value(this->GetChar() < op.GetChar());
                case VINT:
                    return Value(this->GetChar() < op.GetInt());
                default:
                    return Value();
            }
        case VBOOL:
            switch(rightOperandType) {
                case VBOOL:
                    return Value(this->GetBool() < op.GetBool());
                default:
                    return Value();
            }
        case VSTRING:
            switch(rightOperandType) {
                case VSTRING:
                    return Value(this->GetString() < op.GetString());
                default:
                    return Value();
            }
        default:
            return Value();
    }
}

//overloaded equality operator of this with op
Value Value::operator==(const Value& op) const{
    ValType leftOperandType = this->GetType();
    ValType rightOperandType = op.GetType();
    switch (leftOperandType) {
        case VINT:
            switch(rightOperandType) {
                case VINT:
                    return Value(this->GetInt() == op.GetInt());
                case VREAL:
                    return Value(this->GetInt() == op.GetReal());
                case VCHAR:
                    return Value(this->GetInt() == op.GetChar());
                default:
                    return Value();
            }
        case VREAL:
            switch(rightOperandType) {
                case VINT:
                    return Value(this->GetReal() == op.GetInt());
                case VREAL:
                    return Value(this->GetReal() == op.GetReal());
                default:
                    return Value();
            }
        case VCHAR:
            switch(rightOperandType) {
                case VCHAR:
                    return Value(this->GetChar() == op.GetChar());
                case VINT:
                    return Value(this->GetChar() == op.GetInt());
                default:
                    return Value();
            }
        case VBOOL:
            switch(rightOperandType) {
                case VBOOL:
                    return Value(this->GetBool() == op.GetBool());
                default:
                    return Value();
            }
        case VSTRING:
            switch(rightOperandType) {
                case VSTRING:
                    return Value(this->GetString() == op.GetString());
                default:
                    return Value();
            }
        default:
            return Value();
    }
}

//overloaded inequality operator of this with op
Value Value::operator!=(const Value& op) const {
    ValType leftOperandType = this->GetType();
    ValType rightOperandType = op.GetType();
    switch (leftOperandType) {
        case VINT:
            switch(rightOperandType) {
                case VINT:
                    return Value(this->GetInt() != op.GetInt());
                case VREAL:
                    return Value(this->GetInt() != op.GetReal());
                case VCHAR:
                    return Value(this->GetInt() != op.GetChar());
                default:
                    return Value();
            }
        case VREAL:
            switch(rightOperandType) {
                case VINT:
                    return Value(this->GetReal() != op.GetInt());
                case VREAL:
                    return Value(this->GetReal() != op.GetReal());
                default:
                    return Value();
            }
        case VCHAR:
            switch(rightOperandType) {
                case VCHAR:
                    return Value(this->GetChar() != op.GetChar());
                case VINT:
                    return Value(this->GetChar() != op.GetInt());
                default:
                    return Value();
            }
        case VBOOL:
            switch(rightOperandType) {
                case VBOOL:
                    return Value(this->GetBool() != op.GetBool());
                default:
                    return Value();
            }
        case VSTRING:
            switch(rightOperandType) {
                case VSTRING:
                    return Value(this->GetString() != op.GetString());
                default:
                    return Value();
            }
        default:
            return Value();
    }
}

//overloaded logical Anding operator of this with op
Value Value::operator&&(const Value& op) const {
    ValType leftOperandType = this->GetType();
    ValType rightOperandType = op.GetType();
    switch (leftOperandType) {
        case VBOOL:
            switch(rightOperandType) {
                case VBOOL:
                    return Value(this->GetBool() && op.GetBool());
                default:
                    return Value();
            }
        default:
            return Value();
    }
};

//overloaded logical Oring operator of this with op
Value Value::operator||(const Value& op) const {
    ValType leftOperandType = this->GetType();
    ValType rightOperandType = op.GetType();
    switch (leftOperandType) {
        case VBOOL:
            switch(rightOperandType) {
                case VBOOL:
                    return Value(this->GetBool() || op.GetBool());
                default:
                    return Value();
            }
        default:
            return Value();
    }
};

//overloaded logical Complement operator of this object
Value Value::operator!(void) const {
    ValType leftOperandType = this->GetType();
    switch (leftOperandType) {
        case VBOOL:
            return Value(!(this->GetBool()));
        case VINT:
            return Value(!(this->GetInt()));
        default:
            return Value();
    }
}