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
0. type definitions
1. const value definitions
2. static member variables
3. Constructors, Destructors, Assignment Operators
4. member variables
5. member functions
6. set-getters
7. operator overloads
8. exceptions

A category can consist of three subcategories
1. public
2. protected
3. private

subcategories parts are never split.
If a subcategory is empty, the specifier is omitted.

Category subparts are always separated by an empty line.

In each category, function return types and -independently- function names share indentation.

In each category, functions are ordered and optionally grouped based on these criteria:
1. purpose similarity
2. specifier commonality (const, virtual, override)

## Function Format
A function implementation can consist of the following parts:
1. return type
2. namespace
3. name
4. parameter(s)
5. specifier(s)
6. body opening bracket
7. body
8. body closing bracket

- return type exists on the line above the name;
	if it consists of multiple parts, they are delimited with a single space.
- name is optionally preceeded by namespace, separated by two colons (::),
	followed by braces encapsulating all parameters.
- parameters are delimited by a single comma and space.
- specifiers exist on the line below the name,
	each followed by a single space,
	followed by the body opening bracket.
- body starts on the line below body opening bracket.
- body closing bracket exists on the line below the body.

```
int
complicatedStuff(char *dest, char const *src, int n)
specifier1 specifier2 {
	// body
}
```

### Exception:
Trivial functions should not take up too much space,
	they follow this format instead:

- return type, name, parameters, specifiers, body opening bracket exist on one line;
	return type and name are separated by a TAB (\t);
	parameters and specifiers are separated by a single space.
- body (single line) exists on the line below the name.
- body closing bracket exists on the line below the body.

```
int	min(int a, int b) const {
	return ((a < b) ? a : b);
}
```
\*Trivial could mean having a single line body using a single operator or keyword,
	but really that's for yourself to decide.

## Class Function Format
#### In a Class, new features, and thus new rules are introduced.

If a function has no return type, the entire line it would occupy is omitted.

If a function has an empty body, the opening and closing bracket follow each other directly:
```
Class::~Class(int x)
	:	_x(x)
{}
```

Class Constructors can have an initialiser list, it is to be formatted as follows:
- the initialiser list exists on the line below the name, followed by the specifiers.
- the parameters and initialiser list are separated by a colon (:).
- each initialisation exists on a separate line, separated by a comma.
- the colon and commas (delimiters) and -independently- variables share indentation,
	the delimiters have an indentation of 1 tab, the variables 2 tabs.
```
Class::Class(param x)
	:	_x(x)
	,	_y(random())
{}
```
Note that member variables with a default constructor do not need to appear in the initialiser list,
	explicit construction of these variables is not enforced by this style guide.
Member variables must always be initialised in the order in which they are listed in the class declaration.
