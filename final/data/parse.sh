#!/bin/bash
file1=Nehalem.o*
file2=Opteron.o*
file3=Quad.o*
i=-1
cat $file1 | while read line
do
    i=$(($i + 1))
    if (("$i" < "4"))
    then
        echo $line >> "D${file1}_gcc"    
        continue
    fi

    if (("$i" < "8"))
    then
        echo $line >> "D${file1}_scc"    
        continue
    fi

    if (("$i" < "12"))
    then
        echo $line >> "D${file1}_icc"    
        continue
    fi
done

cat $file2 | while read line
do
    i=$(($i + 1))
    if (("$i" < "4"))
    then
        echo $line >> "D${file2}_gcc"    
        continue
    fi

    if (("$i" < "8"))
    then
        echo $line >> "D${file2}_scc"    
        continue
    fi

    if (("$i" < "12"))
    then
        echo $line >> "D${file2}_icc"    
        continue
    fi
done

cat $file3 | while read line
do
    i=$(($i + 1))
    if (("$i" < "4"))
    then
        echo $line >> "D${file3}_gcc"    
        continue
    fi

    if (("$i" < "8"))
    then
        echo $line >> "D${file3}_scc"    
        continue
    fi

    if (("$i" < "12"))
    then
        echo $line >> "D${file3}_icc"    
        continue
    fi
done
