#./check-len.pl < ncopy.yo
#make VERSION=full
#../misc/yis sdriver.yo
./correctness.pl
#(cd ../y86-code; make testpsim)
#(cd ../ptest; make SIM=../pipe/psim )
#(cd ../ptest; make SIM=../pipe/psim TFLAGS=-i)
# ###./correctness.pl -p
./benchmark.pl
