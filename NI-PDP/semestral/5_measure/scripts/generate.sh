# Author: Ondřej Wrzecionko
for i in {25..30}
do
  j=$((17 + $RANDOM % 10))
  real=$([ $i -le $j ] && echo "$i" || echo "$j")
  echo "Generating... $i $real"
  ./generator -t AD -n "$i" -k "$real" -w80,120 -c
  mv graf.out "graph-hard/graf-$i-$real.txt"
  echo "Generated :-)"
done