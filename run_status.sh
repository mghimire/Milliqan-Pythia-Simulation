for f in pythialogfiles${1}/*.txt; do if [ `grep -c "tree weight is" $f ` -eq 0 ]; then echo -n $f; grep "generated" $f|tail -1; fi; done

