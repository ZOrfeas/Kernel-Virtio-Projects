#!/bin/sh

for n in $(seq 0 9);
do
	filename=$(echo bf0$(echo $n));
	dd if=/dev/zero of=$filename bs=1 count=0 seek=1500M;
	./ch8writer $filename;
done

