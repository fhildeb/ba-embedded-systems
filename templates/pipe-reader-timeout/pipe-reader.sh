if [ ! -p fifo1 ]
then
	rm -f fifo1
	mkfifo fifo1
fi

if [ ! -p fifo2 ]
then
	rm -f fifo2
	mkfifo fifo2
fi

if [ ! -p fifo3 ]
then
	rm -f fifo3
	mkfifo fifo3
fi

xterm -e 'cat > fifo1' &
xterm -e 'cat > fifo2' &
xterm -e 'cat > fifo3' &
./pipe-reader fifo1 fifo2 fifo3

