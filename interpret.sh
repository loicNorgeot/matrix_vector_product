#!/bin/bash
#
./clean.sh
scp norgeot@mesu.dsi.upmc.fr:~/codes/*.txt .
ipython histogramme.py
shotwell *.jpg
