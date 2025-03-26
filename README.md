# Syntax Analyzer for Pascal-like Language

## 📖 Project Description


## 🖼️ Screenshots

<div align="center">
  <img src="img/screenshot1.png" alt="Lexer Output" width="45%">
  <img src="img/screenshot2.png" alt="AST Visualization" width="45%">
  
  <img src="img/screenshot3.png" alt="Code Example" width="45%">
  <img src="img/screenshot4.png" alt="Error Handling" width="45%">
  
  <img src="img/screenshot5.png" alt="Project Structure" width="90%">
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
