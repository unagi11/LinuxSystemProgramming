echo $1 > result.txt

for i in $(seq 1 $1)
do
	./a.out -ct < input >> result.txt
done

./average < result.txt