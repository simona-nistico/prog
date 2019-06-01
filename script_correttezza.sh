nasm -f elf32 pqnn32.nasm

for file in $(ls ./assembly | grep .nasm); do
  nasm -f elf32 ./assembly/$file
done

gcc -O0 -m32 -msse pqnn32.o assembly/*.o pqnn32c.c -o pqnn32c -lm

v="Ass_noDist_Divide"

./pqnn32c prova -exaustive -adc -knn 4 > "./TestCorrettezzaAss/test_Ex_noSim_$v.txt"
./pqnn32c prova -s -exaustive -sdc 4 > "./TestCorrettezzaAss/test_Ex_Sim_$v.txt"
./pqnn32c prova -s -noexaustive -adc 4 > "./TestCorrettezzaAss/test_noEx_noSim_$v.txt"
./pqnn32c prova -s -noexaustive -sdc 4 > "./TestCorrettezzaAss/test_noEx_Sim_$v.txt"


#inserita la compilazione del file testindex.c e datatypes.c, anzi tolta
#Inserito comando -lm per  linkare la libreria math.h

#Gestita la compilazione di tutti i file assembly in una volta e
#Linkati al file di c
