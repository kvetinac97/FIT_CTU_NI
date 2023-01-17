#!/bin/bash

rm -f inst/*
rm -f result.dat
echo 'inst;succ A;succ B;of;steps A;steps B;awg fined A;awg fined B;mu A;sig^2 A;mu B;sig^2 B;xing;winner' > result.dat

for i in `seq 1 10000`
do
  NUMOFVARS=$(( 20 + ( RANDOM % 56 ) ))
  NUMOFCLAUSES=$(( 42 * NUMOFVARS / 10 ))
  ./CreateSAT -q -g u -c $NUMOFCLAUSES -v $NUMOFVARS -k 3 -f "inst/gen-$i"
  for j in `seq 1 1000`
  do
    ./gsat2 -r time -p 0.4 -i 1500 "inst/gen-$i.cnf" > /dev/null 2>> "inst/gsat-$i.dat" &
    ./probsat -r time --cb 2.3 -m 1500 "inst/gen-$i.cnf" > /dev/null 2>> "inst/psat-$i.dat" &
  done
  # count of same instances
  echo -n "gen-$i;" >> result.dat
  succA=$(awk -F' ' 'BEGIN { cnt=0 } $3 == $4 { cnt++ } END { print cnt }' "inst/gsat-$i.dat" | tr -d '\n')
  echo -n "$succA;" >> result.dat
  succB=$(awk -F' ' 'BEGIN { cnt=0 } $3 == $4 { cnt++ } END { print cnt }' "inst/psat-$i.dat" | tr -d '\n')
  echo -n "$succB;" >> result.dat
  succMax=$(cat "inst/gsat-$i.dat" | wc -l | tr -d '\n' | tr -d ' ')
  echo -n "$succMax;" >> result.dat
  awk -F' ' 'BEGIN { sum=0 } { sum += $1 } END { print sum / NR }' "inst/gsat-$i.dat" | tr -d '\n' >> result.dat
  echo -n ";" >> result.dat
  awk -F' ' 'BEGIN { sum=0 } { sum += $1 } END { print sum / NR }' "inst/psat-$i.dat" | tr -d '\n' >> result.dat
  echo -n ";" >> result.dat
  awk -F' ' 'BEGIN { sum=0 } { $3 == $4 ? sum += $1 : sum += 10 * $2 } END { print sum / NR }' "inst/gsat-$i.dat" | tr -d '\n' >> result.dat
  echo -n ";" >> result.dat
  awk -F' ' 'BEGIN { sum=0 } { $3 == $4 ? sum += $1 : sum += 10 * $2 } END { print sum / NR }' "inst/psat-$i.dat" | tr -d '\n' >> result.dat
  echo -n ";" >> result.dat
  muA=$(awk -F' ' 'BEGIN { sum=0 } { sum += log($1) } END { print sum / NR }' "inst/gsat-$i.dat" | tr -d '\n')
  echo -n "$muA;" >> result.dat
  awk -F' ' "BEGIN { muA=$muA; sum=0 } { sum += (log(\$1) - muA)^2 } END { print sum / (NR - 1) }" "inst/gsat-$i.dat" | tr -d '\n' >> result.dat
  echo -n ";" >> result.dat
  muB=$(awk -F' ' 'BEGIN { sum=0 } { sum += log($1) } END { print sum / NR }' "inst/psat-$i.dat" | tr -d '\n')
  echo -n "$muB;" >> result.dat
  awk -F' ' "BEGIN { muB=$muB; sum=0 } { sum += (log(\$1) - muA)^2 } END { print sum / (NR - 1) }" "inst/psat-$i.dat" | tr -d '\n' >> result.dat
  echo -n ";" >> result.dat
  totalA=$(awk -F' ' 'BEGIN { sum=0 } { sum += $1 } END { print sum }' "inst/gsat-$i.dat" | tr -d '\n')
  totalB=$(awk -F' ' 'BEGIN { sum=0 } { sum += $1 } END { print sum }' "inst/psat-$i.dat" | tr -d '\n')
  paste "inst/gsat-$i.dat" "inst/psat-$i.dat" | tr '\t' ' ' | awk -F' ' "BEGIN { totalA=$totalA; totalB=$totalB; rateA=$succA/$succMax; rateB=$succB/$succMax; sumA=0; sumB=0; lastA=1; lastB=0; winner=\"A\"; xing=0; } { sumA += \$1; sumB += \$5; nowA=rateA*(sumA/totalA); nowB=rateB*(sumB/totalB); if ((nowA < nowB && lastA > lastB) || (nowB < nowA && lastB > lastA)) { if (nowB < nowA) { winner=\"A\" } else { winner=\"B\" }; xing=NR }; lastA=nowA; lastB=nowB } END { print xing, winner }" | tr -d '\n' | tr ' ' ';' >> result.dat
  echo "" >> result.dat
done
