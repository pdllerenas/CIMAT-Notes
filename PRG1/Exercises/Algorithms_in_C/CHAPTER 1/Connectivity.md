1. Give the output that a connectivity algorithm should produce when
given the input 0-2, 1-4, 2-5, 3-6, 0-4, 6-0, and 1-3.

>[!todo]- Solution
>
> |input|output|
> |----|------|
> |0-2|0-2|
> |1-4| 1-4|
> |2-5|2-5|
> |3-6|3-6|
> |0-4| |
> |6-0| 6-0|
> |1-3||

2. List all the different ways to connect two different objects for the example in Figure 1.1.
>[!todo]- Solution
>  (0,1) None
>  (0,2) 
> - 0-8-4-3-2
> - 0-8-4-9-2
> (0,3)
> - 0-8-4-3
> - 0-8-4-9-2-3
> (0,4)
> - 0-8-4
> (0,5) None
> (0,6) None
> (0,7)
> - 0-8-4-3-7
> - 0-8-9-2-3-7
> (0,8)
> - 0-8
> (0,9)
> - 0-8-4-9

3. Describe a simple method for counting the number of sets remaining after using the union and find operations to solve the connectivity problem as described in the text.

>[!todo]- Solution
>  

4. Show the contents of the id array after each union operation when you use the quick-find algorithm (Program 1.1) to solve the connectivity problem for the sequence 0-2, 1-4, 2-5, 3-6, 0-4, 6-0, and 1-3. Also give the number of times the program accesses the id array for each input pair.
>[!todo]- Solution
>```c
>void quick_find() {
 > int i, p, q, t, id[N];
  >for (i = 0; i < N; i++) {
  >  id[i] = i;
  >}
  >int count[N] = {0};
  >while(scanf("%d %d\n", &p, &q) == 2) {
  >  if (id[p] == id[q])  {
  > 	  count[p]++;
   > 	count[q]++;
   > 	continue;
   > }
  >  for (t = id[p], i = 0; i < N; i++) {
  > 	count[p]++;
   > 	if (id[i] == t) {
  > 		id[i] = id[q];
  > 		count[q]++;
  >	}
  >    count[i]++;
  >  }
  >  print_id(id);
 > }
 > print_id(count);
>}
>```
>```bash
>0 2 
1 4
2 1 2 3 4 5 6 7 8 9 
2 5
2 4 2 3 4 5 6 7 8 9 
3 6
5 4 5 3 4 5 6 7 8 9 
0 4
5 4 5 6 4 5 6 7 8 9 
6 0
4 4 4 6 4 4 6 7 8 9 
1 3
4 4 4 4 4 4 4 7 8 9 
q
28 17 17 17 10 8 17 6 6 6 
>```
>






