#include "Lexer.h"
#include "Parser.h"
#include "AST.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::string inputCode = R"(
const eps = 0.0001;
var a,b: real;
begin
  write('Введите числа a и b (a<b): ');
  readln(a,b);
  assert(a<b);

  var fa := sin(a);
  var fb := sin(b);
  assert(fb*fa<0);

  while (b-a) > eps do
  begin
    var x := (b+a)/2;
    var fx := sin(x);
    if fa*fx <= 0 then
      b := x;
    else
    begin
      a := x;
      fa := fx;
    end;
  end;

  writeln('Корень функции на [a,b] равен ',(b+a)/2);
end.
    )";

    // Можно также читать из файла:
    // std::ifstream inFile("input.txt");
    // std::stringstream buffer;
    // buffer << inFile.rdbuf();
    // std::string inputCode = buffer.str();

    Lexer lexer(inputCode);
    std::vector<Token> tokens = lexer.tokenize();

    std::cout << "Лексический анализ завершён. Токены:" << std::endl;
    for (const auto &token: tokens) {
        std::cout << token.lexeme << " ";
    }
    std::cout << std::endl << "-----------------------" << std::endl;

    try {
        Parser parser(tokens);
        std::shared_ptr<ASTNode> ast = parser.parse();

        std::cout << "Дерево разбора:" << std::endl;
        ast->print();
    } catch (std::exception &ex) {
        std::cerr << "Ошибка: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
