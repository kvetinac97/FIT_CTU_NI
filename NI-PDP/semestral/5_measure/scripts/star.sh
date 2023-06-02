# === COMPILE ===

# Build Sequential solution
g++ -Wall -pedantic main_seq.cpp -O3 -o main_seq

# Build OpenMP solution
g++ -Wall -pedantic -fopenmp main_par_data.cpp -O3 -o main_par

# Build MPI solution
mpic++ main.cpp -O3 -fopenmp -o main_pdp

# === RUN ===

# Start sequential job
qrun 20c 1 pdp_serial scripts/serial_job.sh

for i in {2,4,8,10,16,20} do
  # Start OpenMP jobs
  qrun 20c 1 pdp_serial "scripts/openmp_job$i.sh"

  # Start MPI jobs (master + 2 slaves)
  qrun 20c 3 pdp_long "scripts/parallel_job$i.sh"

  # Start MPI jobs (master + 3 slaves)
  qrun 20c 4 pdp_fast "scripts/parallel_job$i.sh"
done

# === WRITE & CLEAN ===

cat *.sh.* > star.txt
rm *.sh.*
