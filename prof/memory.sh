#!/bin/bash
valgrind --tool=massif ./$1
ms_print massif.out.* > $1.txt
rm massif.out.*
