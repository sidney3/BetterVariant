# A More Ergomic C++ Variant

## Snippet

We offer the pattern matching syntax found in languages like OCaml, used as follows: 
```cpp
sidney3::variant<int, double, string> b = "Hello World";

b ||
    [](const int obj)
    {
        std::cout << "object is an integer\n";
    } 
    | [](const double obj)
    {
        std::cout << "object is a double\n";
    } 
    | [](const string obj)
    {
        std::cout << "object is a string\n";
    };
```

```bash
>object is a string
```

## Guarded matching

If you provide a lambda `f(U) -> sidney3::GuardedReturn<T>`, if your variant holds a value of type `U` but `f(U) = std::nullopt`, this case will be ignored.

Note that `GuardedReturn<T>` inherits from `std::optional<T>`. We just make the types different so that we can have `std::optional<T>` as a type in our variant.

Also Note that it is undefined behavior to have a `guarded_return<T>` as one of your possible variant types.

## Additional Details

The `|` operator joins two callables into an `overloaded` type, and the `||` operator joins a `variant<Ts...>` with a callable type that covers all `Ts...`.

This is functionally equivalent to `std::visit`, while being more ergonomic. Also, note that we return a function, rather than forcing the operation to be immediately invoked. Note however that the functor will carry a pointer to the original variant, and so lifetimes are a potential concern.

## Already Existing Solutions

For a bit of context, this is what the use of `std::visit` looks like to achieve the same code as the above.

```cpp
struct Visitor
{
    void operator()(const std::string s)
    {
        std::cout << "from the string " << s << "\n";
    }
    void operator()(const int s)
    {
        std::cout << "from the string " << s << "\n";
    }
    void operator()(const double s)
    {
        std::cout << "from the string " << s << "\n";
    }
};

std::variant<std::string, int, double> v = "Hello World";
std::visit(Visitor{}, v);
```

This can be made slightly more ergonomic with the following:

```cpp
// boiler plate, include once
template<typename... Ts> struct overload : Ts... {using Ts::operator()...;};
template<typename... Ts> overload(Ts...) -> overload<Ts...>;

std::variant<std::string, int, double> v = "Hello World";

std::visit(overload{
    [](const int obj)
    {
        std::cout << "object is an integer\n";
    },
    [](const double obj)
    {
        std::cout << "object is a double\n";
    },
    [](const string obj)
    {
        std::cout << "object is a string\n";
    };
}, v);
```

## Extra components

Because we are doing non-trivial template metaprogramming (i.e. extracting arguments, return values from callable types), we build out a seperate library `mpl`. The important files in this are `functional.h` and `list_traits.h` that offer utility for working with function types and list types, respectively.
