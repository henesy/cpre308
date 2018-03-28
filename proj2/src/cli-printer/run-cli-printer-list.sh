#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../libprintserver

./cli-printer -l samplec.ps
