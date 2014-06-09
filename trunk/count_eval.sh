#! /bin/bash

sumTarget=0
sumOthers=0 

if [ -z "$1" ] || [ -z "$2" ]
 then
   echo "./count_eval.sh DirName Type"
   exit 1
 else  

   for d in $1/*
      do
        F=$(basename $d)
        F=` echo $F | sed -r "s/\.txt//" `
        numF=`grep -P "$2( |\n)" $d | wc -l`
       #numF=`grep "$2" $d | wc -l`
        if [[ $F =~ ^(gk|sk|zk|nk|tk)$ ]]
          then 
            ((sumTarget = sumTarget + numF))
            echo "$F : $2 = $numF" 
	   else
            ((sumOthers = sumOthers + numF))
            echo "$F : $2 = $numF" 
        fi
      done  
    echo "Целевые $2: $sumTarget"
    echo "Остальные $2: $sumOthers"
fi
