
for p in 1 2 3 4 5 6 7 8 10 15 20 25 30 35 40 50 60 70 80 90 100
do
	export OMP_NUM_THREADS=$p
	(/usr/bin/time -f "%e\t%U" area) &>> output.txt
done