# Parallel multiplication of square matrices

### Task:

There are two files with square matrices of integers 200 by 200. It is required to implement multithreaded multiplication of these matrices. The result should be written to the third file. The number of threads for multiplication is set at start. Working with matrices should be designed in the form of classes with overloaded operators. Working with threads should also be designed in the form of classes.

### Implementation:

To work with threads, the thread pool(matrix/Threadmanager.h) was implemented, which allows you not to waste resources each time on creating threads, but to keep the specified number in working order. Threads are constantly waiting for a new task to appear in the queue, and when it appears, one of them immediately starts its execution. To add a function together with parameters to the queue for execution by the pool, the `create_task` method is implemented, which accepts a functor with a variable list of parameters.

### Testing:

Using the generator file.py in *Python* I generated 9 random pairs of matrices using the **numpy** library, wrote them to the appropriate files. After that, I calculated their product using the same library and also wrote the result to a file.

File test.cpp reads square matrices into the `Matrix` class, calculates the resulting matrix using the written multiplication function and compares what happened in Python and in my implementation

### Generation and compilation:

```
make all
```