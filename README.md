# Introduction
**Forthy-Two** is an esoteric programming language. That means, you can use it
to run computer programs, but shouldn't.

If that does not scare you (or you're actually looking forward to using it,
because you don't know better yet), this document will give you a few hints on
how to build the interpreter.

# Prerequisites
The code depends on the following, rather modest, list of tools.
* cmake (www.cmake.org, \>= 2.8.3, used 3.5.2)
* ninja (www.ninja-build.org, used 1.6.0)
* C++ 98 (g++, used 4.8.5)

Ensure that you install these things if you really want to continue.

In case you want to contribute to **Forthy-Two** (which I can neither
understand nor recommend, basically for your own sanity), you may want to
install
* uncrustify (0.63)
* doxygen (1.8.10)

# Compiling for Developers (Sandbox Mode)
The normal mode of compiling the **Forthy-Two** interpreter (for a very
specific and uncommon interpretation of _normal_), is to run

    ./make.sh

This will compile and install the interpreter to `install/bin/forthytwo`.

All code is built in the sub-folder `build/Release` and the test suite will be run.

If you want to build a debug version, run

    ./make.sh -d

# Compiling for Package Managers
If you really want to include **Forthy-Two** into your Linux distribution,
follow the following steps followed by a follow-up examination by your
psychiatrist. Also be aware, that I will probably stop using your Linux
distribution as soon as I learn about the inclusion of **Forthy-Two** in it.

The, rather unsurprising, sequence of commands is.

    mkdir -p build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr ..

# Running *Forthy-Two* programs
Once you have a compiled and installed interpreter, you can run programs like this

    ./install/bin/forthytwo example/bottles.42

or if you made the mistake to install it globally,

    forthytwo example/bottles.42

This will run the '99 bottles of beer' example, which is mandated to be
included in each and every esoteric programming language. Do not look at the
source code yet. At least not until you actually bought 99 bottles of beer.

# Documentation
The C++ code is documented using Doxygen. The HTML help will be built by

    doxygen

Should you make the life-changing decision (which I assume, will not be for the
better) to actually read the documentation, open `build/doc/html/index.html`.

# Programming in *Forth-Two*

You don't actually want to program in this language. Ever. Trust me. I wrote it.

If you want to understand why, see the tutorial in `doc/tutorial.md` for an
introduction to the language. As soon as you understand the full extent of
this language, you'll see Cthulhu on your toast. Or his lesser known brother,
Bob.

## What does the language actually look like?

You're still reading? OK. You're curious what this language looks like? Hmm, OK. Get
ready. Here it comes:

    13 12 42 10 12 42

Am I kidding? Nope. That's a valid function in **Forthy-Two**. What does it do? It prints a line break.

That is all. There are no funny characters, no colorful pictures, no rhymes, just numbers. Disappointed? Good.

<!---
vim: fileencoding=utf-8 tw=0 spell spelllang=en wrap
-->
