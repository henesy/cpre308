#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../libprintserver

./cli-printer -d color -s description -o first_test.pdf $(pwd)/samplec.ps

./cli-printer -d black_white -s "more exposition" -o second_test.pdf $(pwd)/samplec.ps

