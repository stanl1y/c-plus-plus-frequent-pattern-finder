# c-plus-plus-frequent-pattern-finder
This is a frequent pattern finder implemented in c++. For example, the input is:  
5,9,10  
0,1,4,6,8,10  
0,1,10  
5  
0,1,3,8,10  
9  
0,2,4,5,6,9  
3  
0,4,6,7,9,10  
0,6,8,10  
0,1,5,7,8,9  
0,2,4,9  
1,2,3,5,7,9,10  
0,4,7,9  
0,2,7  
0,2,3,6,7,8,9  
5,7,10  
8  
0,1,4,8,9,10  
0,1,2,9,10  

If the min support is set to be "0.2" ,then it will output:  
0:0.6500  
1:0.3500  
2:0.3000  
3:0.2000  
4:0.3000  
5:0.3000  
6:0.2500  
7:0.3500  
8:0.3500  
9:0.5500  
10:0.5000  
0,1:0.3000  
0,2:0.2500  
0,4:0.3000  
0,6:0.2500  
0,7:0.2500  
0,8:0.3000  
0,9:0.4000  
0,10:0.3500  
1,8:0.2000  
1,9:0.2000  
1,10:0.3000  
2,9:0.2500  
4,9:0.2500  
5,9:0.2000  
7,9:0.2500  
8,10:0.2000  
9,10:0.2500  
0,1,8:0.2000  
0,1,10:0.2500  
0,2,9:0.2000  
0,4,9:0.2500  
0,7,9:0.2000  
0,8,10:0.2000  

execution:  
`g++ -std=c++11 -o main main.cpp`  
`./main [min support] [input file] [out file]`  
EX: `./main 0.2 input1.txt output1.txt`
