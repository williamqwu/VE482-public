ifconfig | awk -e '{for(i=1;i<=NF;i++){ if($i=="inet"){print $(i+1)} } }'
