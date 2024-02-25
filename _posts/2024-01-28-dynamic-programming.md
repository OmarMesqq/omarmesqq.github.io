---
layout: post
title:  "What is dynamic programming?"
date:   2024-01-28 11:00:00 -0300
categories: tech
---
{% include mathjax.html %}

Dynamic programming (DP) is usually defined as a particular way of solving problems by breaking it down into simpler, easier
sub-problems. It's a common pattern in coding platforms like Leetcode and HackerRank even though not always is this nomenclature
present. 

Anyways, how does DP happen in practice and why is it important after all? Let's have a look at one of its main approaches:

#### Memoization (top-down)
A common programming problem in colleges, bootcamps, and coding sites is the calculation of the Fibonacci number at
the `nth` position. In Python, one can do:

```python
def fibonacci(n):
    if n <= 1:
        return n
    else:
        return fibonacci(n - 1) + fibonacci(n - 2)
```

which is the most straightforward solution for the problem. Afterall, the Fibonacci sequence **is** a recursive sequence.
Someone might want to implement an iterative version of this solution — which is perfectly valid since these usually are 
more performant than the respective recursive ones.

Indeed, for production-ready software (which should use large values for `n`) pushing a frame to the call stack 
for each function invocation can drastically affect program performance (for the worse). Moreover, the exponential time
complexity of the recursive algorithm $$ O(n^2) $$ slows it down for large inputs. 

Still, using recursion, especially 
in the context of functional programming, can be more [mathematically elegant][wikipedia-math-elegance] and readable, which raises
another problem: how to optimize recursive solutions? Behold, **memoization**:

```python
def memoize(func):
  cache = {}
  def wrapper(n):
      if n not in cache:
          cache[n] = func(n)
      return cache[n]
  return wrapper

@memoize
def fibonacci(n):
    if n <= 1:
        return n
    else:
        return fibonacci(n - 1) + fibonacci(n - 2)
```

In this code, we define a `memoize` function  that **acts as 
decorator** for the actual Fibonacci algorithm. This means that when `fibonacci` is called, actually, the `wrapper` function is returned, which cuts down algorithmic complexity
to $$ O(n) $$.

This happens because `memoize` closes over the `cache` dictionary. Since the capture "remembers" the cache variable
even when it goes out of scope, for all Fibonacci calculation
invocations the computation's result will be stored for future reuse. This means that if you run `fibonacci(5)` and then `fibonacci(10)` you will get the result mostly in linear time due to the caching of the result for all integers less
or equal than 5.

In conclusion, memoizing functions can greatly reduce program
overhead especially if you are making use of top-down (recursive) solutions in your code. This approach allows 
you to have the readability and beauty of recursive code 
and not sacrifing performance. 


[wikipedia-math-elegance]: https://en.wikipedia.org/wiki/Mathematical_beauty
