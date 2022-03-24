#!/bin/bash

printf '%s\n' "$(date +%Z-%Y-%m-%d_%H-%M-%S)" >> test.csv
df -h | awk '{printf $1","$2","$3","$4","$5","$6"\n"}' >> test.csv
