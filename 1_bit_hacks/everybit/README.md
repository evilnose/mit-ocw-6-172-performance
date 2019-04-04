# Everybit Explanation

## Rotate
Done using this equation as given in the assignment sheet: reversed(reversed(a)reversed(b)) = ba
So rotating a left by k bits is then rotated(rotated(a\_0tok)rotated(a\_k+1toend))

## Count bit flips
XOR each block with itself shifted left by 1 bit. The number of 1's in the resulting number is then the 
number of bit flips. Then we count population of 1's .
