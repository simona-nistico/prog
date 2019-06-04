nasm -f elf32 pqnn32.nasm

for file in $(ls ./assembly | grep .nasm); do
  nasm -f elf32 ./assembly/$file
done

gcc -O0 -m32 -msse pqnn32.o assembly/*.o pqnn32c.c -o pqnn32c -lm

let i=1
v=_0

rm "./TestTempi/v$v.txt"

for (($i;i<=8;i*=2)); do
  echo \n"--------------------KNN: $i-----------------------------"\n  >> ./TestTempi/v1.txt
  ./pqnn32c prova -exaustive -adc -knn $i >> "./TestTempi/v$v.txt"
  ./pqnn32c prova -s -exaustive -sdc -knn $i >> "./TestTempi/v$v.txt"
  ./pqnn32c prova -s -noexaustive -adc -knn $i >> "./TestTempi/v$v.txt"
  ./pqnn32c prova -s -noexaustive -sdc -knn $i >> "./TestTempi/v$v.txt"
done

#inserita la compilazione del file testindex.c e datatypes.c, anzi tolta
#Inserito comando -lm per  linkare la libreria math.h

#Gestita la compilazione di tutti i file assembly in una volta e
#Linkati al file di c
