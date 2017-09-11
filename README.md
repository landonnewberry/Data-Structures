# data_structures
A set of data structures written in C.

## Contents:

- **Integer** (*integer.c*): Used for testing the library. A bare-bones wrapper for the standard *int*. *integer.c* is an example for the general pattern that this library of data structures is built on. An abstract data type, in this case an integer, with a *new*, *display*, and *compare* function. *integer.c* and *integer.h* was written by my former professor Dr. John Lusth.

- **String** (*string.c*): My implementation of a string wrapper for testing the library.

- **Dynamic Array** (*darray.c*): Doubles in size at every fill. The constructor *newDArray* takes a function pointer to the display function of the abstract data type that you want to store in it.

- **Stack** (*stack.c*): Basic implementation with a constructor *newStack* that takes a function pointer to the display function of the abstract data type you want to store on the stack.

- **Queue** (*queue.c*): Basic implementation with a constructor *newQueue* that takes a function pointer to the display function of the abstract data type you want to enqueue.

- **Singly-Linked List** (*sll.c*): Singly-linked list with a tail pointer. Constructor *newSLL* takes a function pointer to the display function of the data type you want to store in the list.

- **Doubly-Linked List** (*dll.c*): Has a tail pointer. Same as *sll.c* except it is doubly-linked so it can remove from the tail in constant time.

- **Binary Search Tree** (*bst.c*): Classic implementation. Does not self-balance in any way.

- **Red-Black Tree** (*rbt.c*): Self-balancing search tree.

- **Binomial Heap** (*binomial.c*): Standard binomial heap implementation.
