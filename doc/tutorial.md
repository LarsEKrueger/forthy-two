# A Tutorial of Sorts for a Programming Language of Sorts

## Introduction

Much like cats are nature's experiment of compensating wickedness by cuteness,
**Forthy-Two** is my experiment of compensating an inherently wicked language
design by test driven development. In some sense, we both succeeded.

**Forthy-Two** is an esoteric programming language, not something you want to
use in production. This should be clear as soon as you have read this
tutorial, but there are certain kinds of people who I suspect to try anyway.
**You have been warned!** (Read this in a throaty, hollow voice.)

In theory, **Forthy-Two** is [Turing
Complete](https://en.wikipedia.org/wiki/Turing_completeness). There is no
formal proof of this, and there probably never will be. So, we stick with the
informal definition: *If a programming language can run a program that prints
the famous '99 bottles of beer' poem, it is Turing Complete*. The program to
do this is to be found in `examples/bottles.42`.

## Computation Model

Like many other esoteric programming languages, **Forthy-Two** is stack-based
language. This makes for a simple interpreter, but requires the programmer to
do some mental juggling. The next few paragraphs illustrate that.

All programs begin with an empty stack. We denote this by
>    [ ]

When the interpreter encounters a number in the program, e.g. 17, it puts it
on top of the stack, resulting in the following
> [ `17` ]

Encountering another number, e.g. 4, it also puts it on the stack.
> [ `17` `4` ]

The addition operation `plus` takes the two top-most numbers off the stack, adds
them and puts the result back:
> [ `17` `4` ]
>
> plus
>
> [ `21` ]

It leaves all other items on the stack in peace:
> [ `1` `2` `3` `17` `4` ]
>
> plus
>
> [ `1` `2` `3` `21` ]

There is a specific number, 42, that is both the answer to life, the universe,
and **Forthy-two**'s name. If the interpreter encounters it, it takes a number
from the stack, and calls that line in the current program.

The first few lines are reserved for built=in functions, e.g. `plus`, which is
associated with line 0. Thus, the following function will leave the number 21
on top of the stack.
> []
>
> 17 4 0 42
>
> [ `21` ]

Let's break it down. We start with the empty stack, then place 17 and 4 on top
of it:
> [ ]
>
> 17 4
>
> [ `17` `4` ]

Now, we put a zero there
> 0
>
> [`17` `4` `0` ]

Finally, we put a 42 there, which takes the zero off the stack and therefore
calls `plus` as before.
> 42
>
> [ `21` ]

Now, what would we do to get a `42`? One option is to compute `4*10 + 2` as
the following program does. Multiplication is done by calling line `2`.
> [ ]
>
> 4 10 2 42 2 0 42
>
> [ `42` ]

Or we could compute `6*6+6`, using the `dup` operation (line 9), which duplicates the
top-of-stack element.
> [ ]
>
> 6
>
> [ `6` ]
>
> 9 42
>
> [ `6` `6` ]
>
> 9 42
>
> [ `6` `6` `6` ]
>
> 2 42
>
> [ `6` `36` ]
>
> 0 42
>
> [ `42` ]

Here is the full table of built-in operations:

|Line number|Operation                          |Stack effect
|----------:|:----------------------------------|:-----------
|0          | Addition                          | [ `a` `b` ] 0 42 [ `a+b` ]
|1          | Subtraction                       | [ `a` `b` ] 1 42 [ `a-b` ]
|2          | Multiplication                    | [ `a` `b` ] 2 42 [ `a*b` ]
|3          | Division                          | [ `a` `b` ] 3 42 [ `a/b` ]
|4          | Modulus                           | [ `a` `b` ] 4 42 [ `a%b` ]
|5          | Logical And                       | [ `a` `b` ] 5 42 [ `a and b` ]
|6          | Logical Or                        | [ `a` `b` ] 6 42 [ `a or b` ]
|7          | Logical Not                       | [ `a` ] 7 42 [ `!a` ]
|8          | Swap                              | [ `a` `b` ] 8 42 [ `b` `a` ]
|9          | Duplicate top-of-stack            | [ `a` ] 9 42 [ `a` `a` ]
|10         | Drop the top-of-stack             | [ `a` ] 10 42 [ ]
|11         | Loop                              | [ `a` ] 11 42 [ ]
|12         | Print a character                 | [ `a` ] 12 42 [ ]
|13         | Read a character from stdin       | [ ] 13 42 [ `a` ]
|14         | Exit the program                  | [ `a` ] 14 42 [ ]
|15         | Duplicate the 2nd element         | [ `a` `b` ] 15 42 [ `a` `b` `a` ]

The only data type is signed 32 bit integers.

## Programming Model

All programs start at line 21 because the line numbers before that are
reserved for built-in operations.

The first twenty lines are typically used for documentation, such as stating
the author (if you dare) and purpose of the program (if it has one). The use
of at least one item of ASCII art is mandatory.

Line 42 can be called just like any other line. You have to compute the line
number though as the literal 42 would cause a call that you don't want.

All other lines are parsed according to the pattern
> \<number\> \<space\> \<number\> ...

Parsing a line is stopped as soon as an illegal number is encountered. This
feature is meant to be abused to the best of your abilities.

If you call a line that is beyond the last line in the source file, execution
jumps to line 21.

## Walkthrough of a Simple Example

We use `examples/euler1.42` to go through a complete program, step by step.

The purpose of the program is to compute the sum of all the multiples of 3 or
5 below 1000.

The useful code with line number, but without any comments is:

|Line|Code|
|---:|:---|
| 21 |58 42 0 999 35 42  38 42 45 42 0 14 42
| 25 |9 42 7 42 7 42 2 2 42 10 0 42 42
| 26 |25 42 11 42
| 29 |9 42 10 4 42 48 0 42 8 42 10 3 42
| 30 |29 42 11 42
| 35 |9 42 53 42 40 0 42 42        1 1 42 11 42
| 38 |0 8 42 30 42 26 42
| 41 |8 42  15 42 0 42 8 42
| 45 |13 12 42 10 12 42
| 48 |4 42 7 42
| 53 |9 42 3 48 42 8 42 5 48 42 6 42
| 58 |0 32 115 105 32 48 48 48 49 32 119 111 108 101 98 32 53 32 114 111 32 51 32 102 111 32 115 101 108 112 105 116 108 117 109 32 108 108 97 32 102 111 32 109 117 115 32 101 104 84 26 42

Let's start at line 21. For easier reading, the separate calls are separated
by slashes.
> 58 42 / 0 999 35 42 / 38 42 / 45 42 / 0 14 42

It consists of a sequence 5 function calls: line 58, line 35 (using 0 and 999
as parameters), line 38, line 45, and exit with code 0.

We start with line 58, which prints a message:
> 0 32 115 105 32 48 48 48 49 32 119 111 108 101 98 32 53 32 114 111 32 51 32 102 111 32 115 101 108 112 105 116 108 117 109 32 108 108 97 32 102 111 32 109 117 115 32 101 104 84 / 26 42

It puts a sequence of number on the stack, then calls line 26 to print a
zero-terminated string. We get to that line (which prints a zero-terminated
string) soon.

Line 35 is the main computation loop:
> 9 42 / 53 42 / 40 0 42 / 42        / 1 1 42 / 11 42

The six instructions are: Duplicate top-of-stack, line 53, add 40, call,
subtract one, loop. As we've seen in line 21, the stack effect of line 35 is:
> [ sum number ] 35 42 [ sum ]

Therefore, line 35 does this:
> [ `sum` `number` ]
>
> 9 42 (duplicate top-of-stack)
>
> [ `sum` `number` `number` ]
>
> line 53 (is number divisible by 3 or 5)
>
> [ `sum` `number` `isDivisible?` ]
>
> 40 0 42 (add 40)
> 
> [ `sum` `number` `40 or 41` ]
>
> 42 (line 40, which does nothing or line 41, which adds `sum` and `number`)
>
> [ `sum` `number` ]
>
> 1 1 42 (subtract one)
>
> [ `sum` `number` ]
>
> 11 42 (loop)
>
> [ `sum` ]

The loop instruction checks if the top-of-stack value is zero. If so, it drops
the number off the stack and continues. If not, it leaves the number and
continues execution at the beginning at the current line. Thus, it loops over
all numbers beginning at 999 and checks them for divisibility and adds them
to the current sum.

Lets have a look at line 53, the actual check:
> 9 42 / 3 48 42 / 8 42 / 5 48 42 / 6 42

> [ `number` ]
>
> 9 42 (duplicate)
>
> [ `number` `number` ]
>
> 3 48 42 (call line 48 to check for divisibility by 3)
>
> [ `number` `isDivisibleBy3` ]
>
> 8 42 (swap top two numbers)
>
> [ `isDivisibleBy3` `number` ]
>
> 5 48 42 (call line 48 to check for divisibility by 5)
>
> [ `isDivisibleBy3` `isDivisibleBy5` ]
>
> 6 42 (logical or)
>
> [ `isDivisibleBy3or5` ]

The divisibility check in line 48 does this:
> [ `number` `divisibleBy` ]
>
> 4 42 ( modulo )
>
> [ `remainder` ]
>
> 7 42 (logical not, i.e. if top-of-stack != 0 return 0, else return 1)
>
> [ `isDivisible` ]

Line 41 accumulates the number to the current sum:
> [ `sum` `number` ]
>
> 8 42 (swap)
>
> [ `number` `sum` ]
>
> 15 42 (over)
>
> [ `number` `sum`  `number` ]
>
> 0 42 (add)
>
> [ `number` `sum+number` ]
>
> 8 42 (swap)
>
> [ `sum+number` `number` ]

This concludes the actual computation to solve Project Euler, problem 1. The
remaining lines are helper functions to convert number to string and to print
a zero-terminated string.

Line 38 converts a number to a zero-terminated string and prints it:
> [ `number` ]
>
> 0
>
> [ `number` 0 ]
>
> 8 42 (swap)
>
> [ 0 `number` ]
>
> 30 42 (convert)
>
> [ 0 `digit` ... ]
>
> 26 42 (print)
>
> [ ]

Line 30 is a simple loop until the number is zero:
> [ `number` ]
>
> 29 42 (convert digit)
>
> [ `digit` `number` ]
>
> 11 42 (loop)
>
> [ `digit` ... ]

The actual digit conversion in line 29 work like this:
> [ `number` ]
>
> 9 42 (dup)
>
> [ `number` `number` ]
>
> 10 4 42 (modulo 10)
>
> [ `number` `lowest digit` ]
>
> 48 0 42 (convert to ASCII)
>
> [ `number` `digit character` ]
>
> 8 42 (swap)
>
> [ `digit character` `number` ]
>
> 10 3 42 ( divide by 10)
>
> [ `digit character` `number/10` ]

Line 26 (which prints a zero-terminated string) again a simple loop to provide
the loop's contents as a testable function. The actual printing is done in
line 25:
> [ 0 ... `character` ]
>
> 9 42 (duplicater)
>
> [ 0 ... `character` `character` ]
>
> 7 42 7 42 (convert to boolean)
>
> [ 0 ... `character` `character is non-zero (0/1)` ]
>
> 2 2 42 (multiply by 2)
>
> [ 0 ... `character` `character is non-zero (0/2)` ]
>
> 10 0 42 (add 10)
>
> [ 0 ... `character` `character is non-zero (10/12)` ]
>
> 42 (call)
>
> [ 0 ... ]

The trick is to use the computed call mechanism to either call 10 (drop) or 12
(emit) to ignore or print the character. Both function remove the top element
from the stack. The outer loop in line 26 will then see the zero terminator
and exit the loop (dropping the zero) or continue with the next character.

Line 45 prints a newline character.


<!---
vim: fileencoding=utf-8 tw=78 spell spelllang=en
-->
