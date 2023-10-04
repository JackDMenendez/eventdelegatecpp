# eventdelegatecpp, A C++ Implementation of .Net Event Delegate Semantics.
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
A library project that implements event delegation semantics in C++ that are similar to
those of C#. The goal is the ability to subscribe to an event via callback. Our goal is to provide something like the following example: 

    int ctr = 0;

    void incrementor(int* ctr) { *ctr += 1; }

    Event text_changed += Delegate(incrementor);

    text_changed += Delegate([](int* ctr) { *ctr += 10; });

    ...

    text_changed(&ctr);

After the above code executed, "ctr" would have a value of 11. There are some challenges to constructing a reliable system of 
delegated functions safely subscribing, un-subscribing, and especially delegates that go out
of scope while subscribed. We also want to support as many kinds of callbacks as possible such
as class methods where "this" has a value and lambdas with closures.

Another goal of this library is to use as much of C++20 and later compilers early binding capabilities.
In other words, do as much at compile time. We want to make firing an event and calling all of its delegates
as fast and efficient as possible including respecting noexcept when it is specified on delegates.

A future goal is supporting C++ coroutines and C++ concurrency.
