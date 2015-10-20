ipcs -s | egrep "0x[0-9]+ [0-9]+" | cut -f2 -d " " | xargs -n1 ipcrm -s
