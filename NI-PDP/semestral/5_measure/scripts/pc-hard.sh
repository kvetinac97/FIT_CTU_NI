g++ -Wall -pedantic main_seq.cpp -O2 -o main_seq
g++ -Wall -pedantic main_par_task.cpp -fopenmp -O2 -o main_par_task
g++ -Wall -pedantic main.cpp -fopenmp -O2 -o main_par_data

for file in graph-hard/*
do
  echo "======================"
  echo "Sequential: $file"
  time ./main_seq < "$file"
  echo "======================"
  echo "Parallel (task): $file"
  time ./main_par_task < "$file"
  echo "======================"
  echo "Parallel (data): $file"
  time ./main_par_data < "$file"
done

echo "======================"
rm main_seq
rm main_par_task
rm main_par_data