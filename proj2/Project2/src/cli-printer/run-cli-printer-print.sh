#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../libprintserver

./cli-printer -d color -s description -o first_test.pdf /tmp/samplec.ps
