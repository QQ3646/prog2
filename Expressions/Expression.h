class Environment;

class Expression {
public:
    virtual Expression *eval(Environment &env) = 0;

    virtual void print() = 0;

    virtual ~Expression() = default;

    virtual bool syntaxCheck() { return true; }
};
