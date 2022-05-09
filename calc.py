# a file for calculating the number of digits in the factorial of a number
import numpy as np

a = np.dot(np.log10(np.arange(1,1000000001)), np.ones([1000000000,1]))

print (a)