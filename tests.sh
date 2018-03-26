for ((i=1; i<26; i++))
do
    if [ $i -lt 10 ]
    then
        printf "Test 0%d: " $i
        ./p1 -ast tiny_test_progs/tiny_0${i} > resultTree0${i}.txt
        DIFF=$(diff resultTree0${i}.txt tiny_test_progs/tiny_0${i}.tree)
    else
        printf "Test %d: " $i
        ./p1 -ast tiny_test_progs/tiny_${i} > resultTree${i}.txt
        DIFF=$(diff resultTree${i}.txt tiny_test_progs/tiny_${i}.tree)
    fi

    if [ "$DIFF" != "" ]
    then
        echo "Failed"
    else
        echo "Success"
    fi
done
