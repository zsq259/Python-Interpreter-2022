#include "Evalvisitor.h"
#include <Any.h>
#include "Python3BaseVisitor.h"
#include "Scope.h"
#include "utils.h"

antlrcpp::Any EvalVisitor::visitFile_input(Python3Parser::File_inputContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx) {
    // no func def
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitParameters(Python3Parser::ParametersContext *ctx) {
    // no func def
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitStmt(Python3Parser::StmtContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) {
    return visitChildren(ctx);
}

//copied from apple-pie
antlrcpp::Any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) {
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

antlrcpp::Any EvalVisitor::visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitTest(Python3Parser::TestContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx) {
    auto andArray = ctx->and_test();
    for (int i = 0; i < andArray.size(); ++i) {
        antlrcpp::Any ret = visitAnd_test(andArray[i]);
        if(ret.is<bool>() && ret.as<bool>() == true) return true;
    }
    return false;
    //return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx) {
    auto notArray = ctx->not_test();
    for (int i = 0; i < notArray.size(); ++i) {
        antlrcpp::Any ret = visitNot_test(notArray[i]);
        if(ret.is<bool>() && ret.as<bool>() == false) return false;
    }
    return true;
    //return visitChildren(ctx);
}

//11.16 upd and waiting for solve
antlrcpp::Any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx) {
    auto notArray = ctx->not_test();
    if (notArray) {
        antlrcpp::Any ret = visitNot_test(notArray);
        if(ret.is<bool>()) return !ret.as<bool>();
    }
    return visitChildren(ctx);
}
//11.16 upd
antlrcpp::Any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx) {
    auto arithArray = ctx->arith_expr();
    if (arithArray.size() == 1) return visitArith_expr(arithArray[0]);
    auto opArray = ctx->comp_op();
    antlrcpp::Any ret = visitArith_expr(arithArray[0]);
    for (int i = 1; i < arithArray.size(); ++i) {
        std::string tmpOp = opArray[i-1]->getText();
        antlrcpp::Any tem = visitArith_expr(arithArray[i]);
        if (tmpOp == "<") {
            if (ret.is<int>() && tem.is<int>() && (ret.as<int>() >= tem.as<int>())) return false;
            else if (ret.is<float>() && tem.is<float>() && (ret.as<float>() >= tem.as<float>())) return false;
            else if (ret.is<std::string>() && tem.is<std::string>() && (ret.as<std::string>() >= tem.as<std::string>())) return false; 
        }
        else if (tmpOp == ">") {
            if (ret.is<int>() && tem.is<int>() && (ret.as<int>() <= tem.as<int>())) return false;
            else if (ret.is<float>() && tem.is<float>() && (ret.as<float>() <= tem.as<float>())) return false;
            else if (ret.is<std::string>() && tem.is<std::string>() && (ret.as<std::string>() <= tem.as<std::string>())) return false; 
        }
        else if (tmpOp == "==") {
            if (ret.is<int>() && tem.is<int>() && (ret.as<int>() != tem.as<int>())) return false;
            else if (ret.is<float>() && tem.is<float>() && (ret.as<float>() != tem.as<float>())) return false;
            else if (ret.is<std::string>() && tem.is<std::string>() && (ret.as<std::string>() != tem.as<std::string>())) return false; 
        }
        else if (tmpOp == ">=") {
            if (ret.is<int>() && tem.is<int>() && (ret.as<int>() < tem.as<int>())) return false;
            else if (ret.is<float>() && tem.is<float>() && (ret.as<float>() < tem.as<float>())) return false;
            else if (ret.is<std::string>() && tem.is<std::string>() && (ret.as<std::string>() < tem.as<std::string>())) return false; 
        }
        else if (tmpOp == "<=") {
            if (ret.is<int>() && tem.is<int>() && (ret.as<int>() > tem.as<int>())) return false;
            else if (ret.is<float>() && tem.is<float>() && (ret.as<float>() > tem.as<float>())) return false;
            else if (ret.is<std::string>() && tem.is<std::string>() && (ret.as<std::string>() > tem.as<std::string>())) return false; 
        }
        else if (tmpOp == "!=") {
            if (ret.is<int>() && tem.is<int>() && (ret.as<int>() == tem.as<int>())) return false;
            else if (ret.is<float>() && tem.is<float>() && (ret.as<float>() == tem.as<float>())) return false;
            else if (ret.is<std::string>() && tem.is<std::string>() && (ret.as<std::string>() == tem.as<std::string>())) return false; 
        }
        ret = tem;
    }
    return true;
    //return visitChildren(ctx);
}

//copied from apple-pie  and   11.16 upd
antlrcpp::Any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx) {
    auto termArray = ctx->term();
    if (termArray.size() == 1) return visitTerm(termArray[0]);
    auto opArray = ctx->addorsub_op();
    int is_int = 1, is_float = 0;
    std::vector<antlrcpp::Any> Array;
    for (int i = 0; i < termArray.size(); ++i) Array.push_back(visitTerm(termArray[i]));
    for (int i = 0; i < termArray.size(); ++i) {
        if (!Array[i].is<int>()) is_int = 0;
        if (Array[i].is<double>()) is_float = 1;
    } 
    if (is_int) {
        int ret = Array[0].as<int>();
        for (int i = 1; i < termArray.size(); ++i) {
            std::string tmpOp = opArray[i-1]->getText();
            if (tmpOp == "+") ret += Array[i].as<int>();
            else if (tmpOp == "-") ret -= Array[i].as<int>();
        }
        return ret;
    }
    else if (is_float) {
        double ret = Array[0].as<double>();
        for (int i = 1; i < termArray.size(); ++i) {
            std::string tmpOp = opArray[i-1]->getText();
            if (tmpOp == "+") ret += Array[i].as<double>();
            else if (tmpOp == "-") ret -= Array[i].as<double>();
        }
        return ret;
    }
    else {
        std::string ret = Array[0].as<std::string>();
        for (int i = 1; i < termArray.size(); ++i) {
            std::string tmpOp = opArray[i-1]->getText();
            if (tmpOp == "+") ret += Array[i].as<std::string>();
        }
        return ret;
    }
}

//copied from apple-pie    and    11.16 upd
antlrcpp::Any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx) {
    auto factorArray = ctx->factor();
    if (factorArray.size() == 1) return visitFactor(factorArray[0]);
    std::vector<antlrcpp::Any> Array;
    auto opArray = ctx->muldivmod_op();
    for (int i = 0; i < factorArray.size(); ++i) Array.push_back(visitFactor(factorArray[i]));
    int is_int = 1, is_float = 0;
    for (int i = 0; i < factorArray.size(); ++i) {
        if (!Array[i].is<int>()) is_int = 0;
        if (Array[i].is<double>()) is_float = 1;
        if (i && opArray[i-1]->getText() == "/") is_int = 0, is_float = 1;
    }
    if (is_int) {
        int ret = Array[0].as<int>();
        for (int i = 1; i < factorArray.size(); ++i) {
            std::string tmpOp = opArray[i-1]->getText();
            if (tmpOp == "*") ret *= Array[i].as<int>();
            else if(tmpOp == "//") ret /= Array[i].as<int>();
            else if(tmpOp == "%") ret %= Array[i].as<int>();
        }
        return ret;
    }
    else if (is_float) {
        double ret = Array[0].as<double>();
        for (int i = 1; i < factorArray.size(); ++i) {
            std::string tmpOp = opArray[i-1]->getText();
            if (tmpOp == "*") ret *= Array[i].as<double>();
            else if(tmpOp == "/") ret /= Array[i].as<double>();
        }
        return ret;
    }
    else {
        std::string ret = Array[0].as<std::string>();
        for (int i = 1; i < factorArray.size(); ++i) {
            std::string tmpOp = opArray[i-1]->getText();
            if (tmpOp == "*") {
                int tmp = Array[i].as<int>();
                while (tmp) ret += ret; 
            }
        }
        return ret;
    }
}

//copied from apple-pie
antlrcpp::Any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) {
    return visitChildren(ctx);
}

//copied from apple-pie
antlrcpp::Any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) {
    if (!ctx->trailer()) return visitAtom(ctx->atom());
}

//copied from apple-pie
antlrcpp::Any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) {
    if (ctx->arglist()) return visitArglist(ctx->arglist());
    return std::vector<int>();
}

//copied from apple-pie   and   changed
antlrcpp::Any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
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

antlrcpp::Any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx) {
    return visitChildren(ctx);
}

//copied from apple-pie
antlrcpp::Any EvalVisitor::visitArglist(Python3Parser::ArglistContext *ctx) {
    auto argumentArray = ctx->argument();
    std::vector<int> retArray;
    for (auto ctx : argumentArray) {
        retArray.push_back(visitTest(ctx->test()[0]));
    }
    return retArray;
}

antlrcpp::Any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx) {
    return visitChildren(ctx);
}
