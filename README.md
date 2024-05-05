# A Better C++ Variant

## More Ergonomic Use

```cpp
using sidney3::xvariant;

xvariant<int, double, string> b = "Hello World";

b >> 
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
    }();
```
