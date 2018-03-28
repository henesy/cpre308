#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../libprintserver

./cli-printer -d black_white -s description -o first_test.pdf /tmp/samplec.ps
