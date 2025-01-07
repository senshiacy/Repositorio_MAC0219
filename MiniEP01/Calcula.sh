#!/bin/bash

echo "Python,C" > Resultados.csv

get_time() {
  date +%s%N | cut -b1-16
}

for (( i=0; i<100; i++ )) do
	#echo "teste " $i >> Resultados.csv
	#echo "Python:" >> Resultados.csv
	start_time=$(get_time)
	#{ time python3 ${py} > /dev/null; } 2>> Resultados.csv
	python3 $1 > /dev/null
	end_time=$(get_time)
	echo -n "$((end_time - start_time))," >> Resultados.csv
	#echo "C:" >> Resultados.csv
	
	start_time=$(get_time)
	gcc -Wall $2 -lm -o programa
	./programa > /dev/null
	end_time=$(get_time)
	echo -n "$((end_time - start_time))" >> Resultados.csv
	echo -e >> Resultados.csv
	#{ time ./programa > /dev/null; } 2>> Resultados.csv
	#echo -e >> Resultados.csv
done
