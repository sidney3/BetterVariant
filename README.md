# A More Ergomic C++ Variant

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

The `>>` operator joins a `variant` with any callable object, and then further `||` operators will join with this callable to cover all of the cases of the `variant`. The functor cannot be called unless all cases are covered (will not compile).

This is functionally equivalent to `std::visit`, while being more ergonomic.
