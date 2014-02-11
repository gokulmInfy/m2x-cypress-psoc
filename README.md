Cypress PSOC M2X API Client
========================

The Cypress PSoC client library is used to send/receive data to/from [AT&amp;T's M2X service](https://m2x.att.com/) from a [Cypress PSoC 4 board](http://cypress.com/go/CY8CKIT-042). In order to let the PSoC 4 board connect to the Internet, an [Arduino Ethernet shield](http://arduino.cc/en/Main/ArduinoEthernetShield) should be attached to the Cypress PSoC 4 board.

**NOTE**: By default, the J12 headers on the Cypress PSoC 4 board are unpopulated. In order to use the Arduino Ethernet shield and run the library, those headers must be populated. By populating, we mean that those headers must be [soldered](https://learn.sparkfun.com/tutorials/how-to-solder---through-hole-soldering) using some [Break Away Headers](https://www.sparkfun.com/products/116). Otherwise the Ethernet shield will not function.

Getting Started
==========================
1. Signup for an [M2X Account](https://m2x.att.com/signup).
2. Obtain your _Master Key_ from the Master Keys tab of your [Account Settings](https://m2x.att.com/account) screen.
2. Create your first [Data Source Blueprint](https://m2x.att.com/blueprints) and copy its _Feed ID_.
3. Review the [M2X API Documentation](https://m2x.att.com/developer/documentation/overview).

Please consult the [M2X glossary](https://m2x.att.com/developer/documentation/glossary) if you have questions about any M2X specific terms.

How to run the examples
=======================

First, you need to clone the repository and the submodules:

```
$ git clone https://github.com/attm2x/m2x-cypress-psoc
$ cd m2x-cypress-psoc
$ git submodule update --init
```

The code is grouped as a [Cypress PSoC Creator](http://www.cypress.com/psoccreator/) project. You need at least PSoC Creator 3 to open the project.

When you have the PSoC Creator 3 installed, open the workspace at `M2XCypressPSoC/M2XCypressPSoC.cywrk`, you can find 3 projects available:

* `E2ForLife_W5100`: The library used to manipulate the Arduino Ethernet library, which gives us Internet access
* `Jsonlite`: Json parser used in our library
* `M2XCypressPSoC`: Sample project with M2X library packed. This sample project can be used to post values to a stream in a feed.

Since the M2X library depends on the generated code from hardware configuration in PSoC Creator, we now pack the library in the sample project. In the future we might split M2X library files in its own project.

In `main.cpp` file, you need to configure your AT&amp;T M2X master key, the feed and stream to use in order to run the example. When you are done with the changes, you are ready to build and run the example.

Right click on `M2XCypressPSoC` project, and select `Build M2XCypressPSoC`, the PSoC Creator will build all 3 projects, since `M2XCypressPSoC` depends on the other two. When the building process is completed, you can connect your PSoC 4 board using a USB cable, and select `Debug` to run the example on your board.

Floating Point Support
======================

You may notice that in the example, we are posting integers to M2X. If you simply modify the code and post floating point numbers to M2X, you will get `400 Bad Request` errors. This is because PSoC Creator 3+ uses `newlib-nano` as the libc implementation. By default, newlib-nano weak references floating point implementation in printf-like functions to reduce code size. Since we use snprintf to prepare the data to post, floating point numbers cannot be posted.

If you want to enable floating point support in printf, you can use the following steps:

1. Right click on `M2XCypressPSoC` project.
2. Select `Build Settings`.
3. Select `ARM GCC 4.7.3`.
4. Expand `Linker`.
5. Select `Command Line`
6. In `Custom Flags`, add: `-u _printf_float`.
7. Clean and rebuild the project, you should now be able to post floating point numbers.
