jobs=5 # The number of jobs allowed to submit is less than 5
while true
do
	curr=`squeue | grep kliang1 | wc -l`
	if [[ $curr -eq 0 ]]
	then
		exit
	elif [[ $curr -lt $jobs ]]
	then
		squeue | grep kliang1 | head -1
		jobs=$curr
	fi
	sleep 30
done
