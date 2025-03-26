# Синтаксический анализатор Pascal

## 📖 Project Description

Этот проект представляет собой синтаксический анализатор (парсер) для упрощённого Pascal-подобного языка.
Реализованы:

Лексический анализатор (разбивает исходный код на токены).

Синтаксический анализатор (строит абстрактное синтаксическое дерево — AST).

Поддержка базовых конструкций:

Объявление переменных (var), констант (const).

Условные операторы (if-then-else).

Циклы (while-do).

Ввод/вывод (readln, writeln).

Локальные переменные с инициализацией (var x := value).

Простые арифметические и логические выражения.


## 🖼️ Screenshots

<div align="center">
  <img src="img/ob.png" alt="">
  <img src="img/usc.png" alt="" >
  
  <img src="img/cl.png" alt="">

</div>

## 🛠️ Technologies
- **Language**: C++20
- **Build System**: CMake
- **Key Components**:
  - `Lexer.h/cpp` - Token generation
  - `Parser.h/cpp` - Syntax analysis
  - `AST.h` - Abstract Syntax Tree
  - `BoundedDeque.h` - Utility container

## 🚀 Getting Started
```bash
git clone https://github.com/BogdanPryadko4853/syntax-analyzer-pascal.git
cd syntax-analyzer-pascal
mkdir build && cd build
cmake .. && cmake --build .
./syntax-analyzer-pascal
