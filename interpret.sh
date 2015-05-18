#!/bin/bash
#
./clean.sh
scp attouche@mesu.dsi.upmc.fr:~/codes/*.txt .
ipython histogramme.py
shotwell *.jpg
