#! /bin/bash
# pipe this script into the stdin of the print server
# ./test.sh | ./printserver -o log_file.log --n1 2 --n2 4


echo "NEW"
echo "NAME output1.pdf"
echo "FILE acidpaper.ps"
echo "DESCRIPTION From a language"
echo "PRINTER black_white"
echo "PRINT"
sleep 5
echo "NEW"
echo "NAME output2.pdf"
echo "FILE samplec.ps"
echo "DESCRIPTION The second test file"
echo "PRINTER color"
echo "PRINT"
sleep 5
echo "NEW"
echo "NAME output3.pdf"
echo "FILE mkfiles.ps"
echo "DESCRIPTION Mkfiles"
echo "PRINTER black_white"
echo "PRINT"
sleep 5
echo "NEW"
echo "NAME output4.pdf"
echo "FILE pathstar.ps"
echo "DESCRIPTION Inferno OS"
echo "PRINTER color"
echo "PRINT"
sleep 5
echo "EXIT"

