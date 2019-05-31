echo "Differenze stampe tra assembly e c"

#diff test_Ex_noSim_Ass_noDist.txt test_Ex_noSim_noAss.txt  #| grep -c "\-\-\-" #>> echo
#diff test_Ex_Sim_Ass_noDist.txt test_Ex_Sim_noAss.txt  #| grep -c "\-\-\-" #>> echo
#diff test_noEx_noSim_Ass_noDist.txt test_noEx_noSim_noAss.txt  #| grep -c "\-\-\-" #>> echo
diff test_noEx_Sim_Ass_noDist.txt test_noEx_Sim_noAss.txt  #| grep -c "\-\-\-" #>> echo
