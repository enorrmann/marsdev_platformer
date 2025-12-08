#!/usr/bin/env bash

# Default values
input1="${1:-in1.csv}"
input2="${2:-in2.csv}"
output="${3:-out.csv}"

awk -F',' '
NR==FNR {
    # Load first CSV into array
    for(i=1;i<=NF;i++) a[FNR,i]=$i
    nf[FNR]=NF
    next
}
{
    # Compare and output max per column
    n = (NF > nf[FNR] ? NF : nf[FNR])
    for(i=1;i<=n;i++){
        v1 = a[FNR,i] + 0
        v2 = $i + 0
        if(i>1) printf ","
        printf (v1 > v2 ? v1 : v2)
    }
    printf "\n"
}
' "$input1" "$input2" #> "$output"
