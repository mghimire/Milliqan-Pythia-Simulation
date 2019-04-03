
echo "mCP mass"
grep "Z BR" pythialogfiles/gammaZ_*_1.txt | sed s%pythialogfiles/gammaZ_%% | sort -n | cut -d '_' -f 1
echo "Z BR to mCP"
grep "Z BR" pythialogfiles/gammaZ_*_1.txt | sed s%pythialogfiles/gammaZ_%% | sort -n | cut -d ' ' -f 6

echo "mCP mass"
grep " branching ratio approximation " pythialogfiles/qcd_*_1.txt | sed "s%pythialogfiles/qcd_%%" | sort -n | cut -f 1 -d '_'
echo "hadron pdgid"
grep " branching ratio approximation " pythialogfiles/qcd_*_1.txt | sed "s%pythialogfiles/qcd_%%" | sort -n | cut -f 9 -d ' '
echo "BR to mCPs"
grep " branching ratio approximation " pythialogfiles/qcd_*_1.txt | sed "s%pythialogfiles/qcd_%%" | sort -n | cut -f 11 -d ' ' | sed "s%<%0%"
