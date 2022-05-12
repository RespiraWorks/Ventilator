platformio requires that each test get its own `test_[feature]` directory.

> PIO Unit Testing Engine will treat the source code of `test_[feature]` folder as
> SINGLE test, which compiles all `cpp` files that are in the same folder as `test_[feature]`
> If you have only one test, you may create a `test_[feature]` directory (containing
> at least one `cpp` file which includes `gtest_main.h` to define `main()`) directly at the root of
> `test` folder.
> Otherwise, feel free to hierarchise your test files in `test/feature` folder, containing
> its own a `test_[feature]` folder, with one `cpp` file defining `main()` (`#include "gtest_main.h"`).
https://docs.platformio.org/en/latest/advanced/unit-testing/structure.html#test-hierarchy
