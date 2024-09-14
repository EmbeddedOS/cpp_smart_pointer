#include "shared_ptr.hh"

#include <iostream>
#include <memory>
int main()
{
    std::weak_ptr<int> f;
    lava::shared_ptr<int> ptr(new int{3});
    lava::shared_ptr<int> ptr2 = lava::make_shared<int>(4);
    ptr.swap(ptr2);

    std::cout << *ptr << *ptr2 << ptr2.use_count() << std::endl;

    ptr2.reset();

    std::cout << *ptr << ptr2.use_count() << std::endl;
}