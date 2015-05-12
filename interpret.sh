#!/bin/bash
#
rm *.jpg
scp norgeot@mesu.dsi.upmc.fr:~/codes/computation_times*.txt .
ipython histogramme.py
shotwell *.jpg
rm computation_times*.txt
