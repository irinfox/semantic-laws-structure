#! /bin/bash

if [ -z "$1" ] || [ -z "$2" ]
 then
   echo "./run_toma.sh PlaintextDirName ResTXTDirName"
   exit 1
 else  

  mkdir -p $2
  rm -f $2/*
  for d in $1/*
    do
      TXT=$(basename $d)
      echo $TXT
      D=`echo ${d//\//\\\/}`
      echo $D 
      sed -i "s/Dir = \".*\"/Dir = \"$D\"/" config_legal.proto
      #./tomita-linux64 config_legal.proto > $2/$TXT.txt
      ./tomita-upd config_legal.proto > $2/$TXT.txt
    done
fi
