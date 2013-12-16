#! /bin/bash

#Если есть diff, значит, что-то изменилось
for i in ./test_set1/*.txt
do
  cat $i | ./tomita-linux64 config_test.proto > out.txt
  FACTFN=` echo $i | sed -r "s/\.txt//" `
  diff $FACTFN.facts out.txt

done
