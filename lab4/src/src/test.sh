#! /bin/bash
# pipe this script into the stdin of the print server
# ./test.sh | ./printserver -o log_file.log --n1 2 --n2 4


echo "NEW"
echo "NAME output1.pdf"
echo "FILE samplec.ps"
echo "DESCRIPTION The first test file"
echo "PRINTER black_white"
echo "PRINT"
sleep 5
echo "EXIT"
