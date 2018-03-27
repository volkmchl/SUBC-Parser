for i in $(seq -f "%02g" 1 25)
do
    printf "Test %s: " $i
    ./p1 -ast tiny_test_progs/tiny_${i} > resultTree${i}.txt
    DIFF=$(diff resultTree${i}.txt tiny_test_progs/tiny_${i}.tree)

    if [ "$DIFF" != "" ]
    then
        echo "Failed"
    else
        echo "Success"
    fi
done
