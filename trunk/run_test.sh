for i in ./test-set/*.txt
do
  cat $i | ../../tom config.proto
  FACTFN=` echo $i | sed -r "s/\.txt//" `
  diff $FACTFN.facts out.txt

done
