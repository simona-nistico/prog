echo "Numero di punti più vicini errati"

diff bruteforce.txt prova_e_s.txt | grep -c "\-\-\-" >> echo
diff bruteforce.txt prova_e_ns.txt | grep -c "\-\-\-" >> echo
diff bruteforce.txt prova_ne_s.txt | grep -c "\-\-\-" >> echo
diff bruteforce.txt prova_ne_ns.txt | grep -c "\-\-\-" >> echo
