platformio requires that each test get its own `test_[feature]` directory.

> PIO Unit Testing Engine will treat the source code of `test_[feature]` folder as a
> SINGLE test, which compiles all `cpp` files that are in the same folder as `test_[feature]`
> One (and only one) of those `cpp` file needs to include `gtest_main.h` in order to define `main()`).
https://docs.platformio.org/en/latest/advanced/unit-testing/structure.html#test-hierarchy
