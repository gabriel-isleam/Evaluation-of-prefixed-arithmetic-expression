# Evaluation-of-prefixed-arithmetic-expression
Evaluation of prefixed expression using expression trees


The algorithm reads from an input file a fixed number of variables, their values and forms with them a simple linked list. Then, it reads from the same file a number (x) which represents the number of expression to solve. After that, it reads x expressions for which it recursively forms the expression tree with the help of some functions, such as SonsNumber, which for "sqrt" returns 1, that means a single possible son, 2 for the rest of the operators and 0 for the operands which represents the leaves of the tree.

The evaluation of the tree is also done recursevely, calculating for each node, the result from the left subtree, then the result from the right subtree and between the two is performed the operation indicated by the symbol of the node. Cases in which evaluation is stopped are those where a previously unlabeled variable is found in the tree or it is required the square root of a negative number or the case of divison by 0.
