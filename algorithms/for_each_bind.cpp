// Demonstrating a for_each loop that invokes a bound function
// instead of a custom loop and an if operation
#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

struct Foo {
    Foo(int i) : id_(i) { }
    void Bar(int i) { std::cout << "Bar " << i << " at " << id_ << std::endl; }
    void Baz(int i) { std::cout << "Baz " << i << " at " << id_ << std::endl; }
    int id_;
};

int main(int argc, char** argv)
{
    Foo foo0(0), foo1(1), foo2(2);
    auto fn = std::bind(argc == 2 ? &Foo::Bar : &Foo::Baz, std::placeholders::_1, 1);
    fn(foo0);
    std::vector<Foo> foos { 1, 2, 3, 4, 5 };
    std::for_each(foos.begin(), foos.end(), fn);
    std::vector<Foo*> fooPtrs { &foo0, &foo1, &foo2 };
    std::for_each(fooPtrs.begin(), fooPtrs.end(), fn);
    return 0;
}
