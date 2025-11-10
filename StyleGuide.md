# Style Guide

no snake_case

## Casing
1. type definition:				PascalCase
2. const value definition:		PascalCase
3. variable:					camelCase
4. private member variable:		_<case> (add a leading underscore)
5. file:						PascalCase

## Header
Every header must start with an include guard with the format:
```
#ifndef <FILENAME>_<EXTENSION>
# define <FILENAME>_<EXTENSION>
// header body
#endif
```
Nothing may be defined outside of this include guard.
Other headers may be included only at the top of the header body.
They will be grouped based on these criteria:
1. <standard C header>
2. <standard C++ header>
3. "library header"
3. "internal header"

Conditional preprocessor statements are indented with a single compounding space per depth level.

## Class Declaration
A class may consist of the following categories, defined in the specified order from top to bottom:
1. const value definitions
2. static member variables
3. Constructors, Destructors, Assignment Operators
4. member variables
5. member functions
6. set-getters
7. operator overloads

Each category consists of optional public and private subparts, these parts are never split.
If either of these parts is empty, the specifier is omitted.

Category subparts are always separated by an empty line.

In each category, function return types and independently function names share indentation.

In each category, functions are ordered and optionally grouped based on these criteria:
1. purpose similarity
2. specifier commonality (const, virtual, override)

## Function Format
A function implementation can consist of the following parts:
1. return type
2. namespace
3. name
4. parameter(s)
5. qualifier(s)
6. body opening bracket
7. body
8. body closing bracket

- return type exists on the line above the name;
	if it consists of multiple parts, they are delimited with a single space.
- name is optionally preceeded by namespace, separated by two colons (::),
	followed by braces encapsulating all parameters.
- parameters are delimited by a single comma and space.
- qualifiers exist on the line below the name,
	each followed by a single space,
	followed by the body opening bracket.
- body starts on the line below body opening bracket.
- body closing bracket exists on the line below the body.

```
return type
name(parameter 1, parameter 2)
qualifier1 qualifier2 {
	// body
}
```

### Exception:
Functions with a single line body should not take up that much space,
	they follow this format instead:

- return type, name, parameters, qualifiers, body opening bracket exist on one line;
	return type and name are separated by a TAB (\t);
	parameters and qualifiers are separated by a single space.
- body (single line) exists on the line below the name.
- body closing bracket exists on the line below the body.

```
return type	name(parameter 1, parameter 2) qualifier1 qualifier2 {
	// body
}
```

## Examples:
```
SourceFile.cpp:
#include <iostream>

const static	int TheAnswer = 42;
static			int	globalVariable;

void
print(std::string message) {
	std::cout << message << "\n";
}
```