# Common code

This is code common to both the cycle controller and the GUI software. Some behavior ought to be intentionally and explicitly the same, such as things related communications protocol and interpretation of physical units.

## Contents

* [libs](libs) - these are custom authored libraries that will be shared by both pieces of deployed software. They should all be tested.
* [generated_libs](generated_libs) - communications protocol definitions and the generated serialization code based thereon, used by both pieces of deployed software, but needs not be tested
* [third_party](third_party) - third party code, also shared, but not unit tested
* [test](test) - these are unit tests for the shared libraries
* [test_libs](test_libs) - these are libraries only used for the purpose of testing, but are themselves not part of the deployed code
* [common.sh](common.sh) - general utility script for running tests and reporting test coverage
* [platformio.ini](platformio.ini) - something like a "make file" which governs how platformio builds the tests

## Building and testing

There is nothing here to build as an executable, but unit tests should be run. We are using the platformio toolchain to do this, just like for the controller. For notes on installing platformio and integrating it with your IDE, see the [controller](../controller) documentation page.
