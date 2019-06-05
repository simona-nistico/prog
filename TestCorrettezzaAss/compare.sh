echo "Differenze stampe tra assembly e c"

diff test_Ex_noSim_risultati_Assembly.txt test_Ex_noSim_risultati_NoAssembly.txt  #| grep -c "\-\-\-" #>> echo
diff test_Ex_Sim_risultati_Assembly.txt test_Ex_Sim_risultati_NoAssembly.txt  #| grep -c "\-\-\-" #>> echo
diff test_noEx_noSim_risultati_Assembly.txt test_noEx_noSim_risultati_NoAssembly.txt  #| grep -c "\-\-\-" #>> echo
diff test_noEx_Sim_risultati_Assembly.txt test_noEx_Sim_risultati_NoAssembly.txt  #| grep -c "\-\-\-" #>> echo
