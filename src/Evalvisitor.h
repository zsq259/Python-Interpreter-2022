#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H


#include "Python3BaseVisitor.h"
#include "Scope.h"
#include "utils.h"

class EvalVisitor: public Python3BaseVisitor {

//todo:override all methods of Python3BaseVisitor
public:
    Scope scope;

    virtual antlrcpp::Any visitFile_input(Python3Parser::File_inputContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitFuncdef(Python3Parser::FuncdefContext *ctx) override {
        // no func def
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitParameters(Python3Parser::ParametersContext *ctx) override {
        // no func def
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitStmt(Python3Parser::StmtContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) override {
        return visitChildren(ctx);
    }

    //copied from apple-pie
    virtual antlrcpp::Any visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) override {
        if (ctx->augassign()) {
        }
        auto testlistArray = ctx->testlist();
        if (testlistArray.size() == 1) {
            visitTestlist(testlistArray[0]);
            return 0;
        }
        else if (testlistArray.size() > 2) {
        }
        
        std::string varName = testlistArray[0]->getText();
        antlrcpp::Any varData = visitTestlist(testlistArray[1]);
        scope.varRegister(varName, varData);
        return 0;
    }

    virtual antlrcpp::Any visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitIf_stmt(Python3Parser::If_stmtContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitSuite(Python3Parser::SuiteContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitTest(Python3Parser::TestContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitOr_test(Python3Parser::Or_testContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitAnd_test(Python3Parser::And_testContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitNot_test(Python3Parser::Not_testContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitComparison(Python3Parser::ComparisonContext *ctx) override {
        return visitChildren(ctx);
    }

    //copied from apple-pie  and   changed
    virtual antlrcpp::Any visitArith_expr(Python3Parser::Arith_exprContext *ctx) override {
        auto termArray = ctx->term();
        if (termArray.size() == 1) return visitTerm(termArray[0]);
        auto opArray = ctx->addorsub_op();
        int is_int = 1, is_float = 0;
        for (int i = 0; i < termArray.size(); ++i) {
            if (!visitTerm(termArray[i]).is<int>()) is_int = 0;
            if (visitTerm(termArray[i]).is<double>()) is_float = 1;
        } 
        if (is_int) {
            int ret = visitTerm(termArray[0]).as<int>();
            for (int i = 1; i < termArray.size(); ++i) {
                std::string tmpOp = opArray[i-1]->getText();
                if (tmpOp == "+") ret += visitTerm(termArray[i]).as<int>();
                else if (tmpOp == "-") ret -= visitTerm(termArray[i]).as<int>();
            }
            return ret;
        }
        else if (is_float) {
            double ret = visitTerm(termArray[0]).as<double>();
            for (int i = 1; i < termArray.size(); ++i) {
                std::string tmpOp = opArray[i-1]->getText();
                if (tmpOp == "+") ret += visitTerm(termArray[i]).as<double>();
                else if (tmpOp == "-") ret -= visitTerm(termArray[i]).as<double>();
            }
            return ret;
        }
        else {
            std::string ret = visitTerm(termArray[0]).as<std::string>();
            for (int i = 1; i < termArray.size(); ++i) {
                std::string tmpOp = opArray[i-1]->getText();
                if (tmpOp == "+") ret += visitTerm(termArray[i]).as<std::string>();
            }
            return ret;
        }
    }

    //copied from apple-pie    and    changed
    virtual antlrcpp::Any visitTerm(Python3Parser::TermContext *ctx) override {
        auto factorArray = ctx->factor();
        if (factorArray.size() == 1) return visitFactor(factorArray[0]);
        auto opArray = ctx->muldivmod_op();
        int is_int = 1, is_float = 0;
        for (int i = 0; i < factorArray.size(); ++i) {
            if (!visitFactor(factorArray[i]).is<int>()) is_int = 0;
            if (visitFactor(factorArray[i]).is<double>()) is_float = 1;
            if (i && opArray[i-1]->getText() == "/") is_int = 0, is_float = 1;
        }
        if (is_int) {
            int ret = visitFactor(factorArray[0]).as<int>();
            for (int i = 1; i < factorArray.size(); ++i) {
                std::string tmpOp = opArray[i-1]->getText();
                if (tmpOp == "*") ret *= visitFactor(factorArray[i]).as<int>();
                else if(tmpOp == "//") ret /= visitFactor(factorArray[i]).as<int>();
                else if(tmpOp == "%") ret %= visitFactor(factorArray[i]).as<int>();
            }
            return ret;
        }
        else if (is_float) {
            double ret = visitFactor(factorArray[0]).as<double>();
            for (int i = 1; i < factorArray.size(); ++i) {
                std::string tmpOp = opArray[i-1]->getText();
                if (tmpOp == "*") ret *= visitFactor(factorArray[i]).as<double>();
                else if(tmpOp == "/") ret /= visitFactor(factorArray[i]).as<double>();
            }
            return ret;
        }
        else {
            std::string ret = visitFactor(factorArray[0]).as<std::string>();
            for (int i = 1; i < factorArray.size(); ++i) {
                std::string tmpOp = opArray[i-1]->getText();
                if (tmpOp == "*") {
                    int tmp = visitFactor(factorArray[i]).as<int>();
                    while (tmp) ret += ret; 
                }
            }
            return ret;
        }
    }

    //copied from apple-pie
    virtual antlrcpp::Any visitFactor(Python3Parser::FactorContext *ctx) override {
        return visitChildren(ctx);
    }

    //copied from apple-pie
    virtual antlrcpp::Any visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override {
        if (!ctx->trailer()) return visitAtom(ctx->atom());
    }

    //copied from apple-pie
    virtual antlrcpp::Any visitTrailer(Python3Parser::TrailerContext *ctx) override {
        if (ctx->arglist()) return visitArglist(ctx->arglist());
        return std::vector<int>();
    }

    //copied from apple-pie   and   changed
    virtual antlrcpp::Any visitAtom(Python3Parser::AtomContext *ctx) override {
        if (ctx->NUMBER()) {
            return (ctx->NUMBER()->getText());
        }
        else if (ctx->NAME()) {
            auto result = scope.varQuery(ctx->NAME()->getText());
            if (result.first) {
                return result.second;
            }
        }
        else if (ctx->test()) return visitTest(ctx->test());
    }

    virtual antlrcpp::Any visitTestlist(Python3Parser::TestlistContext *ctx) override {
        return visitChildren(ctx);
    }

    //copied from apple-pie
    virtual antlrcpp::Any visitArglist(Python3Parser::ArglistContext *ctx) override {
        auto argumentArray = ctx->argument();
        std::vector<int> retArray;
        for (auto ctx : argumentArray) {
            retArray.push_back(visitTest(ctx->test()[0]));
        }
        return retArray;
    }

    virtual antlrcpp::Any visitArgument(Python3Parser::ArgumentContext *ctx) override {
        return visitChildren(ctx);
    }
};


#endif //PYTHON_INTERPRETER_EVALVISITOR_H
