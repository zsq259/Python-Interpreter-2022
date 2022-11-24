#include "Evalvisitor.h"
#include <Any.h>
#include "Python3BaseVisitor.h"
#include "Scope.h"
#include "utils.h"
#include "any_op.h"
#include "int2048.h"
using sjtu::int2048;

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
    //return 
    antlrcpp::Any ret = visitChildren(ctx);
    return ret;
}

antlrcpp::Any EvalVisitor::visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) {
    antlrcpp::Any ret = visitSmall_stmt(ctx->small_stmt());
    return ret;
}

antlrcpp::Any EvalVisitor::visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) {
    antlrcpp::Any ret = visitChildren(ctx);
    return ret;
}

// refer to apple-pie
antlrcpp::Any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) {
    auto testlistArray = ctx->testlist();
    if (ctx->augassign()) {
        std::string op = ctx->augassign()->getText();
        std::string varName = testlistArray[0]->getText();
        antlrcpp::Any varData = visitTestlist(testlistArray[1]);
        if (op == "+=") {
            scope.varRegister(varName, scope.varQuery(varName).second + varData);
        }
        else if (op == "-=") {
            scope.varRegister(varName, scope.varQuery(varName).second - varData);
        }
        else if (op == "*=") {
            scope.varRegister(varName, scope.varQuery(varName).second * varData);
        }
        else if (op == "/=") {
            scope.varRegister(varName, scope.varQuery(varName).second / varData);
        }
        else if (op == "//=") {
            scope.varRegister(varName, int_div(scope.varQuery(varName).second, varData));
        }
        else if (op == "%=") {
            scope.varRegister(varName, scope.varQuery(varName).second % varData);
        }
        return 0;
    }
    if (testlistArray.size() == 1) {
        visitTestlist(testlistArray[0]);
        return 0;
    }
    else {
        int tmp = testlistArray.size();
        //antlrcpp::Any varData = visitTestlist(testlistArray[tmp - 1]);
        std::vector< std::vector<antlrcpp::Any> > Array;
        for (int i = 0; i < tmp; ++i) {
            antlrcpp::Any ret = visitTestlist(testlistArray[i]);
            //std::cout<<"size="<<ret.size()<<"\n";
            /*
            auto testArray = testlistArray[i]->test();
            for (int j = 0; j < testArray.size(); ++j) {
                std::cout<<"j="<<j<<"\n";
                //ret.push_back(
                visitTest(testArray[j]);
            }
            */
            if (ret.is<std::vector<antlrcpp::Any> >()) Array.push_back(ret.as<std::vector<antlrcpp::Any> >());
            else {
                std::vector<antlrcpp::Any> tem;
                tem.push_back(ret);
                Array.push_back(tem);
            }
        }
        for (int i = 0; i < tmp - 1; ++i) {
            for (int j = 0; j < Array[i].size(); ++j) {
                antlrcpp::Any varName = Array[i][j];
                //std::cout<<varName.is<std::pair<std::string, antlrcpp::Any> >()<<'\n';
                varName = varName.as<std::pair<std::string, antlrcpp::Any> >().first;
                antlrcpp::Any varData = (Array.back())[j];
                transVar(varData);
                //std::cout<<varData<<std::endl;
                scope.varRegister(varName, varData);        
            }
        }
        /*

        for (int i = tmp - 2; i >= 0; i--) {
            std::string varName = testlistArray[i]->getText();
            scope.varRegister(varName, varData);        
        }
        */
    }
    /*
    else {
        std::string varName = testlistArray[0]->getText();
        antlrcpp::Any varData = visitTestlist(testlistArray[1]);
        scope.varRegister(varName, varData);
    }
    */
    return 0;
}

antlrcpp::Any EvalVisitor::visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) {
    if (ctx->continue_stmt()) return 1;
    if (ctx->break_stmt()) { return 2; }
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx) {
    auto testArray = ctx->test();
    auto suiteArray = ctx->suite();
    for (int i = 0; i < testArray.size(); ++i) {
        if (visitTest(testArray[i]).as<bool>()) {
            return visitSuite(suiteArray[i]);
        }
    }
    if(testArray.size() < suiteArray.size()) return visitSuite(suiteArray.back());
    return 0;
}

antlrcpp::Any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx) {
    auto Test = ctx->test();
    auto Suite = ctx->suite();
    while(visitTest(Test)) {
        antlrcpp::Any ret = visitSuite(Suite);
        transVar(ret);
        //if (ret.is<int>()) std::cout<<"retttt="<<ret.as<int>()<<std::endl;
        if (ret.is<int>() && ret.as<int>() == 2  ) break;
    }
    return 0;
}

antlrcpp::Any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx) {
    if(ctx->simple_stmt()) return visit(ctx->simple_stmt());
    auto Array = ctx->stmt();
    for (int i = 0; i < Array.size(); ++i) {
        antlrcpp::Any ret = visitStmt(Array[i]);
        transVar(ret);
        if (ret.is<int>()){
            //std::cout<<"ret="<<ret.as<int>()<<'\n';
            if (ret.as<int>() == 1) return 1;//continue
            if (ret.as<int>() == 2) return 2; //break;
        }    
    }
    return 0;
}

antlrcpp::Any EvalVisitor::visitTest(Python3Parser::TestContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx) {
    auto andArray = ctx->and_test();
    if(andArray.size() == 1) return visitAnd_test(andArray[0]);
    for (int i = 0; i < andArray.size(); ++i) {
        antlrcpp::Any ret = visitAnd_test(andArray[i]);
        transVar(ret);
        if(ret.is<bool>() && ret.as<bool>() == true) return true;
    }
    return false;
    //return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx) {
    auto notArray = ctx->not_test();
    if(notArray.size() == 1) return visitNot_test(notArray[0]);
    for (int i = 0; i < notArray.size(); ++i) {
        antlrcpp::Any ret = visitNot_test(notArray[i]);
        transVar(ret);
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
        transVar(ret);
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
    transVar(ret);
    for (int i = 1; i < arithArray.size(); ++i) {
        std::string tmpOp = opArray[i-1]->getText();
        antlrcpp::Any tem = visitArith_expr(arithArray[i]);
        transVar(tem);
        if (tmpOp == "<") { if (ret >= tem) return false; }
        else if (tmpOp == ">") { if (ret <= tem) return false; }
        else if (tmpOp == "==") { if (ret != tem) return false; }
        else if (tmpOp == ">=") { if (ret < tem) return false; }
        else if (tmpOp == "<=") { if (ret > tem) return false; }
        else if (tmpOp == "!=") { if (ret == tem) return false; }
        ret = tem;
    }
    return true;
    //return visitChildren(ctx);
}

// refer to apple-pie  and   11.17 upd
antlrcpp::Any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx) {
    auto termArray = ctx->term();
    if (termArray.size() == 1) return visitTerm(termArray[0]);
    auto opArray = ctx->addorsub_op();
    std::vector<antlrcpp::Any> Array;
    for (int i = 0; i < termArray.size(); ++i) Array.push_back(visitTerm(termArray[i]));
    antlrcpp::Any ret = Array[0];
    transVar(ret);
    for (int i = 1; i < termArray.size(); ++i) {
        std::string tmpOp = opArray[i-1]->getText();
        transVar(Array[i]);
        if (tmpOp == "+") ret = ret + Array[i];
        else ret = ret - Array[i];
    } 
    return std::move(ret);
}

// refer to apple-pie    and    11.17 upd
antlrcpp::Any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx) {
    auto factorArray = ctx->factor();
    if (factorArray.size() == 1) return visitFactor(factorArray[0]);
    std::vector<antlrcpp::Any> Array;
    auto opArray = ctx->muldivmod_op();
    for (int i = 0; i < factorArray.size(); ++i) Array.push_back(visitFactor(factorArray[i]));
    antlrcpp::Any ret = Array[0];
    transVar(ret);
    for (int i = 1; i < factorArray.size(); ++i) {
        std::string tmpOp = opArray[i-1]->getText();
        transVar(Array[i]);
        if (tmpOp == "*") ret = ret * Array[i];
        else if (tmpOp == "/") ret = ret / Array[i];
        else if (tmpOp == "//") ret = int_div(ret, Array[i]);
        else if (tmpOp == "%") ret = ret % Array[i];
    }
    return std::move(ret);
}

// refer to apple-pie
antlrcpp::Any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) {
    antlrcpp::Any ret = visitChildren(ctx);
    //std::cout<<"ret="<<ret.is<int2048>()<<'\n';
    if (ctx->MINUS()) {
        if(ret.is<double>()) ret = -ret.as<double>();
        if(ret.is<int2048>()) ret = int2048(0) - ret.as<int2048>();
    }
    return std::move(ret);
}

// refer to apple-pie and 11.17 upd
antlrcpp::Any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) {
    if (!ctx->trailer()) return visitAtom(ctx->atom());
    auto functionName = ctx->atom()->getText();
    auto argsArray = visitTrailer(ctx->trailer()).as<std::vector<antlrcpp::Any> >();
    if (functionName == "print") {
        for (int i = 0; i < argsArray.size(); ++i) {
            if(argsArray[i].is<double>()){
                printf("%.6lf ",argsArray[i].as<double>());
            }
            else std::cout<<argsArray[i]<<" ";
        }
        std::cout<<"\n";
        return 0;
    }
    if (functionName == "int") {
        return ToInt(argsArray[0]);
    }
    if (functionName == "float") {
        return ToFloat(argsArray[0]);
    }
    if (functionName == "str") {
        return ToString(argsArray[0]);
    }
    if (functionName == "bool") {
        return ToBool(argsArray[0]);
    }
}

// refer to apple-pie
antlrcpp::Any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) {
    if (ctx->arglist()) return visitArglist(ctx->arglist());
    return std::vector<antlrcpp::Any>();
}

// refer to apple-pie   and   11.17upd
antlrcpp::Any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
    if (ctx->NUMBER()) {
        std::string str = ctx->NUMBER()->getText();
        //std::cout<<"str="<<str<<std::endl;
        if (str.find(".") == std::string::npos) return int2048(str);
        else return std::stod(str);
    }
    else if (ctx->NAME()) {
        auto result = scope.varQuery(ctx->NAME()->getText());
        //if (result.first) {
            return make_pair(ctx->NAME()->getText(), result.second);
        //}
        //else return make_pair(ctx->NAME()->getText(), 0);
    }
    else if (ctx->TRUE()) return true;
    else if (ctx->FALSE()) return false;
    else if (ctx->test()) return visitTest(ctx->test());
    else {
        auto stringArray = ctx->STRING();
        std::string ret = "";
        for (int i = 0; i < stringArray.size(); ++i) {
            std::string tem = stringArray[i]->getText();
            tem = tem.substr(1, tem.size() - 2);
            ret = ret + tem;
        }
        return std::move(ret);
    }
}

antlrcpp::Any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx) {
    auto testArray = ctx->test();
    if(testArray.size() == 1) return visitTest(testArray[0]);
    std::vector<antlrcpp::Any> ret;
    for (int i = 0; i < testArray.size(); ++i) ret.push_back(visitTest(testArray[i]));
    return std::move(ret);
}

// refer to apple-pie  ans   11.17 upd
antlrcpp::Any EvalVisitor::visitArglist(Python3Parser::ArglistContext *ctx) {
    auto argumentArray = ctx->argument();
    std::vector<antlrcpp::Any> retArray;
    for (int i = 0; i < argumentArray.size(); ++i) {
        antlrcpp::Any ret = visitArgument(argumentArray[i]);
        transVar(ret); 
        retArray.push_back(ret);
    }
    return std::move(retArray);
}

antlrcpp::Any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx) {
    return visitChildren(ctx);
}
