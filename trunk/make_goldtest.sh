#! /bin/bash

#Файл для генерации правильных тестов
for i in ./test_set1/*.txt
do
  FACTFILE=`echo $i | sed -r "s/\.txt//"`
  cat $i | ./tomita-linux64 config_test.proto > $FACTFILE.facts
done
