#ifndef COMP6771_EXPR_H
#define COMP6771_EXPR_H

#include <memory>

class expr {
public:
    virtual double eval() const = 0;
    virtual ~expr() = default;
};

class plus : public expr {
public:
    plus(std::unique_ptr<expr> lhs,  std::unique_ptr<expr> rhs)
    : _lhs(std::move(lhs)),
      _rhs(std::move(rhs)) {};
    double eval() const override {
        return _lhs->eval() + _rhs->eval();
    }
private:
    std::unique_ptr<expr> _lhs;
    std::unique_ptr<expr> _rhs;
};

class minus : public expr {
public:
    minus(std::unique_ptr<expr> lhs,  std::unique_ptr<expr> rhs)
    : _lhs(std::move(lhs)),
      _rhs(std::move(rhs)) {};
    double eval() const override {
        return _lhs->eval() - _rhs->eval();
    }
private:
    std::unique_ptr<expr> _lhs;
    std::unique_ptr<expr> _rhs;
};

class multiply : public expr {
public:
    multiply(std::unique_ptr<expr> lhs,  std::unique_ptr<expr> rhs)
    : _lhs(std::move(lhs)),
      _rhs(std::move(rhs)) {};
    double eval() const override {
        return _lhs->eval() * _rhs->eval();
    }
private:
    std::unique_ptr<expr> _lhs;
    std::unique_ptr<expr> _rhs;
};

class divide : public expr {
public:
    divide(std::unique_ptr<expr> lhs,  std::unique_ptr<expr> rhs)
    : _lhs(std::move(lhs)),
      _rhs(std::move(rhs)) {};
    double eval() const override {
        return _lhs->eval() / _rhs->eval();
    }
private:
    std::unique_ptr<expr> _lhs;
    std::unique_ptr<expr> _rhs;
};

class literal : public expr {
public:
    literal(double val)
    : _val(val) {};
    double eval() const override {
        return _val;
    }
private:
    double _val;
};



#endif // COMP6771_EXPR_H
