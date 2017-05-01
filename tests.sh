#!/bin/sh
for ((i=1; i<10; i++))
do
    echo "Test 0$i"
   ./p1 -ast tiny_test_progs/tiny_0${i} > userTree0${i}.txt
   diff userTree0${i}.txt tiny_test_progs/tiny_0${i}.tree;
done

for ((i=0; i<10; i++))
do
    echo "Test 1$i"
    ./p1 -ast tiny_test_progs/tiny_1$i > userTree1${i}.txt
    diff userTree1${i}.txt tiny_test_progs/tiny_1${i}.tree;
done

for ((i=1; i<6; i++))
do
    echo "Test 2$i"
    ./p1 -ast tiny_test_progs/tiny_2$i > userTree2${i}.txt
    diff userTree2${i}.txt tiny_test_progs/tiny_2${i}.tree;
done
