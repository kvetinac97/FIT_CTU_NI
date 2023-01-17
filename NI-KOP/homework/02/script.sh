for i in $(eval echo {1..$2})
do
  printf "$1 " >>sol.txt;
  java -jar sawsat.jar $3 $4 $5 $6 $7 $8 $9 < $1 >>sol.txt 2>>out.txt
done
