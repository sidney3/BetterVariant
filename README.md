# A More Ergomic C++ Variant

## Snippet

We offer the pattern matching syntax found in languages like OCaml, used as follows: 
```cpp
sidney3::variant<int, double, string> b = "Hello World";

(b >> 
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
    })();
```

```bash
>object is a string
```

```cpp
sidney3::variant<int, double, string> b = "Hello World";

auto fn = b >> 
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

fn();
```
```bash
>object is a string
```

## Additional Details

The `>>` operator joins a `variant` with any callable object, and then further `||` operators will join with this callable to cover all of the cases of the `variant`. The functor cannot be called unless all cases are covered (will not compile).

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

