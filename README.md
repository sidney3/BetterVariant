# A Better C++ Variant

## More Ergonomic Use

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

/*

Interesting question: how do we want to handle references?

In general -> remove the reference from the lambda

What about constness?
*/
