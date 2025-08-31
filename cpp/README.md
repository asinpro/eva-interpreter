# Eva Interpreter - C++ Port

A complete port of the Eva interpreter from JavaScript to C++. Eva is a Lisp-style programming language with S-expression syntax, supporting functional programming, object-oriented programming, and a module system.

## Features

### Core Language
- **S-expression syntax**: Lisp-style syntax with atoms and lists
- **Variables**: Declaration with `var`, assignment with `set`
- **Functions**: Lambda expressions with closures
- **Control flow**: `if` conditionals, `while` loops, `begin` blocks
- **Built-in functions**: Arithmetic, comparison, and I/O operations

### Syntactic Sugar
- **Function definitions**: `(def name (params) body)`
- **For loops**: `(for init condition update body)`
- **Switch statements**: `(switch (cond1 block1) ... (else blockN))`
- **Increment/decrement**: `++`, `--`, `+=`, `-=`

### Object-Oriented Programming
- **Classes**: `(class Name Parent body)`
- **Instantiation**: `(new ClassName args...)`
- **Properties**: `(prop instance name)` and `(set (prop instance name) value)`
- **Inheritance**: `(super ClassName)` for parent class access

### Module System
- **Module declaration**: `(module name body)`
- **Module import**: `(import name)` loads from `modules/name.eva`
- **Module access**: Property-based access to exported functions and variables

## Building

### Requirements
- C++17 compatible compiler (GCC, Clang, MSVC)
- CMake 3.14 or later

### Build Instructions

```bash
cd cpp
mkdir build
cd build
cmake ..
make
```

### Running Tests

```bash
# Run all tests
make test

# Run individual test suites
./test_environment
./test_parser  
./test_eva
./test_transformer
./test_class
./test_module
```

## Usage

### Command Line Interface

```bash
# Evaluate an expression
./eva -e "(print (+ 2 3))"

# Run a file
./eva -f program.eva
```

### Examples

#### Basic Arithmetic
```lisp
(+ 1 2)                    ; 3
(* (+ 2 3) 4)             ; 20
```

#### Variables
```lisp
(var x 10)
(set x 20)
x                         ; 20
```

#### Functions
```lisp
(def square (x) (* x x))
(square 5)                ; 25

(var factorial 
  (lambda (n) 
    (if (< n 2) 
        1 
        (* n (factorial (- n 1))))))
(factorial 5)             ; 120
```

#### Control Flow
```lisp
(if (> 5 3) 
    "greater" 
    "less")               ; "greater"

(var sum 0)
(for (var i 1) (<= i 10) (++ i)
  (+= sum i))
sum                       ; 55
```

#### Object-Oriented Programming
```lisp
(class Point null
  (begin
    (def constructor (this x y)
      (begin
        (set (prop this x) x)
        (set (prop this y) y)))
    (def distance (this)
      (+ (prop this x) (prop this y)))))

(var p (new Point 3 4))
((prop p distance) p)     ; 7
```

#### Inheritance
```lisp
(class Point3D Point
  (begin
    (def constructor (this x y z)
      (begin
        ((prop (super Point3D) constructor) this x y)
        (set (prop this z) z)))
    (def distance (this)
      (+ ((prop (super Point3D) distance) this)
         (prop this z)))))

(var p3d (new Point3D 1 2 3))
((prop p3d distance) p3d) ; 6
```

#### Modules
```lisp
; Define a module
(module Math
  (begin
    (def abs (x) (if (< x 0) (- x) x))
    (def square (x) (* x x))
    (var PI 3.14159)))

; Use module functions
((prop Math abs) -5)      ; 5
(prop Math PI)            ; 3.14159

; Import from file
(import Math)             ; Loads modules/Math.eva
((prop Math abs) -10)     ; 10
```

## Built-in Functions

### Arithmetic
- `+` - Addition
- `-` - Subtraction (unary and binary)
- `*` - Multiplication  
- `/` - Division
- `%` - Modulo

### Comparison
- `<` - Less than
- `<=` - Less than or equal
- `>` - Greater than
- `>=` - Greater than or equal
- `==` - Equality

### I/O
- `print` - Print values to console

### Special Forms
- `var` - Variable declaration
- `set` - Variable assignment
- `if` - Conditional expression
- `while` - Loop
- `begin` - Block expression
- `lambda` - Function expression
- `def` - Function definition (syntactic sugar)
- `class` - Class declaration
- `new` - Object instantiation
- `prop` - Property access
- `super` - Parent class access
- `module` - Module declaration
- `import` - Module import

## Architecture

The interpreter consists of several key components:

### Environment
- Manages variable scoping with lexical environments
- Supports parent environment chains for nested scopes
- Methods: `define`, `lookup`, `assign`, `resolve`

### Parser
- Recursive descent parser for S-expressions
- Converts text into Abstract Syntax Tree (AST)
- Supports atoms (numbers, strings, symbols) and lists

### AST Nodes
- `NumberNode` - Integer literals
- `StringNode` - String literals  
- `SymbolNode` - Variable names and operators
- `ListNode` - S-expressions (function calls, special forms)

### Transformer
- Converts syntactic sugar into core language constructs
- `def` → `var` + `lambda`
- `for` → `while` loops
- `switch` → nested `if` statements
- Increment/decrement operators → `set` expressions

### Evaluator
- Recursive evaluator implementing the language semantics
- Pattern matching on expression types
- Environment-based variable resolution
- Support for closures and first-class functions

## Differences from JavaScript Version

The C++ port maintains full compatibility with the JavaScript version while adding:

1. **Type Safety**: Static typing with `std::any` for dynamic values
2. **Memory Management**: Smart pointers for automatic memory management
3. **Performance**: Compiled execution vs interpreted JavaScript
4. **Error Handling**: C++ exceptions for runtime errors

## File Structure

```
cpp/
├── CMakeLists.txt        # Build configuration
├── include/              # Header files
│   ├── AST.h            # AST node definitions
│   ├── Environment.h    # Environment class
│   ├── Eva.h            # Main interpreter class
│   ├── Parser.h         # Parser interface
│   └── Transformer.h    # AST transformer
├── src/                 # Implementation files
│   ├── AST.cpp
│   ├── Environment.cpp
│   ├── Eva.cpp
│   ├── Parser.cpp
│   └── Transformer.cpp
├── bin/                 # CLI application
│   └── eva.cpp
├── tests/               # Unit tests
│   ├── test_environment.cpp
│   ├── test_parser.cpp
│   ├── test_eva.cpp
│   ├── test_transformer.cpp
│   ├── test_class.cpp
│   └── test_module.cpp
├── modules/             # Eva module files
│   └── Math.eva
└── build/               # Build output (generated)
```

## Contributing

This is a port of the original Eva interpreter. To contribute:

1. Ensure all existing tests pass
2. Add tests for new features
3. Follow the existing code style
4. Update documentation as needed

## License

Same license as the original Eva interpreter project.