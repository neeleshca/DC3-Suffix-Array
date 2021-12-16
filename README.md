Assignment for Advanced Algorithms (UE16CS311)
The following papers were referred to:<br>
J. Kärkkäinen et al. Linear Work Suffix Array Construction<br>
T. Kasai et al. Linear-Time Longest-Common-Prefix Computation in Suffix Arrays and Its Applications<br>
U. Manber and G. Myers. Suffix arrays: A new method for on-line string searches



There are 2 input files, one is the data with which the suffix array is constructed.  (data.txt)
The second contains the strings to search for.

It is compiled with
g++ main.cpp code.cpp

and run with
./a.out < {input_file}.txt

Input file :
First line contains T, which is the number of test cases
The subsequent T lines each contain one substring to be searched for.

e.g. file named input.txt<br>
1<br>
bunny<br>
