platformio requires that each test get its own directory.

> Place a test in the `test` directory. If you have more than one test, split
> them into sub-folders. For example, `test/test_1/*.[c,cpp,h]`,
> `test_N/*.[c,cpp,h]`, etc. If there is no such directory in the `test` folder,
> then PIO Unit Testing Engine will treat the source code of `test` folder as
> SINGLE test.
https://docs.platformio.org/en/latest/plus/unit-testing.html
