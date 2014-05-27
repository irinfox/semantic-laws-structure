#! /bin/bash

if [ -z "$1" ] || [ -z "$2" ]
 then
   echo "./choose_for_eval.sh PlaintextDirName ResTXTDirName"
   exit 1
 else  

   for d in $1/*
      do
        D=$(basename $d)
        echo $D 
        mkdir -p $2/$D
        echo "$2/$D" 
        if [[ $D =~ ^(gk|sk|zk|nk|tk)$ ]]
          then 
            for f in `ls $d | sort -R | head -n 5`
              do
                cp $d/$f $2/$D
                echo $f
              done
          else
            for f1 in `ls $d | sort -R | head -n 1`
              do 
                cp $d/$f1 $2/$D
                echo $f1
              done
        fi
      done      
fi
