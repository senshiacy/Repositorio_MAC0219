#!/bin/bash

echo "C1,C2" > Resultados.csv

get_time() {
  date +%s%N | cut -b1-16
}

for (( i=0; i<100; i++ )) do
	gcc -Wall $1 -lm -o programa
	start_time=$(get_time)
	./programa > /dev/null
	end_time=$(get_time)
	echo -n "$((end_time - start_time))," >> Resultados.csv
	
	gcc -Wall $2 -O3 -lm -o programa
	start_time=$(get_time)
	./programa > /dev/null
	end_time=$(get_time)
	echo -n "$((end_time - start_time))" >> Resultados.csv
	echo -e >> Resultados.csv
	#{ time ./programa > /dev/null; } 2>> Resultados.csv
	#echo -e >> Resultados.csv
done
