#!/bin/bash

xterm -hold -fa 'Monospace' -fs 16 -e make s7 &
sleep 4
xterm -hold -fa 'Monospace' -fs 16 -e make c71 &
xterm -hold -fa 'Monospace' -fs 16 -e make c72 &
