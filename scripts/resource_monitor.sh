#!/bin/sh

TAD_PATH="/usr/ccsp/tad/"
UTOPIA_PATH="/etc/utopia/service.d"
rebootDeviceNeeded=0
rebootNeededforbrlan1=0
batteryMode=0

source $UTOPIA_PATH/log_env_var.sh
source $TAD_PATH/corrective_action.sh
source /etc/device.properties
source /etc/log_timestamp.sh

exec 3>&1 4>&2 >>$SELFHEALFILE 2>&1

DELAY=30
threshold_reached=0
SELFHEAL_ENABLE=`syscfg get selfheal_enable`

sysevent set atom_hang_count 0

while [ $SELFHEAL_ENABLE = "true" ]
do
	RESOURCE_MONITOR_INTERVAL=`syscfg get resource_monitor_interval`
	if [ "$RESOURCE_MONITOR_INTERVAL" = "" ]
	then
		RESOURCE_MONITOR_INTERVAL=15
	fi 
	RESOURCE_MONITOR_INTERVAL=$(($RESOURCE_MONITOR_INTERVAL*60))

	sleep $RESOURCE_MONITOR_INTERVAL
	
	totalMemSys=`free | awk 'FNR == 2 {print $2}'`
	usedMemSys=`free | awk 'FNR == 2 {print $3}'`
	freeMemSys=`free | awk 'FNR == 2 {print $4}'`

	timestamp=`getDate`

	# Memory info reading using free linux utility

	AvgMemUsed=$(( ( $usedMemSys * 100 ) / $totalMemSys ))

	MEM_THRESHOLD=`syscfg get avg_memory_threshold`

	if [ "$AvgMemUsed" -ge "$MEM_THRESHOLD" ]
	then

		echo_t "RDKB_SELFHEAL : Total memory in system is $totalMemSys at timestamp $timestamp"
		echo_t "RDKB_SELFHEAL : Used memory in system is $usedMemSys at timestamp $timestamp"
		echo_t "RDKB_SELFHEAL : Free memory in system is $freeMemSys at timestamp $timestamp"
		echo_t "RDKB_SELFHEAL : AvgMemUsed in % is  $AvgMemUsed"
		vendor=`getVendorName`
		modelName=`getModelName`
		CMMac=`getCMMac`
		timestamp=`getDate`

		echo "<$level>CABLEMODEM[$vendor]:<99000006><$timestamp><$CMMac><$modelName> RM Memory threshold reached"
		
		threshold_reached=1

		#echo_t "Setting Last reboot reason"
		reason="MEM_THRESHOLD"
		rebootCount=1
		#setRebootreason $reason $rebootCount

		rebootNeeded RM MEM $reason $rebootCount
	fi
	# Avg CPU usage reading from /proc/stat
#	AvgCpuUsed=`grep 'cpu ' /proc/stat | awk '{usage=($2+$4)*100/($2+$4+$5)} END {print usage }'`
#	AvgCpuUsed=`echo $AvgCpuUsed | cut -d "." -f1`
#	IdleCpuVal=`top -bn1  | head -n10 | grep "CPU:" | cut -c 34-35`

#	LOAD_AVG=`cat /proc/loadavg`
#	echo "[`getDateTime`] RDKB_LOAD_AVERAGE : Load Average is $LOAD_AVG"

#	AvgCpuUsed=$((100 - $IdleCpuVal))
#	echo "[`getDateTime`] RDKB_CPU_USAGE : CPU usage is $AvgCpuUsed"

#Record the start statistics

	STARTSTAT=$(getstat)
	
	user_ini=`echo $STARTSTAT | cut -d 'x' -f 1`
	system_ini=`echo $STARTSTAT | cut -d 'x' -f 3`
	idle_ini=`echo $STARTSTAT | cut -d 'x' -f 4`
	iowait_ini=`echo $STARTSTAT | cut -d 'x' -f 5`

#echo "[`getDateTime`] RDKB_SELFHEAL : Initial CPU stats are"
#echo "user_ini: $user_ini system_ini: $system_ini idle_ini=$idle_ini iowait_ini=$iowait_ini"
	sleep $DELAY

#Record the end statistics
	ENDSTAT=$(getstat)

	user_end=`echo $ENDSTAT | cut -d 'x' -f 1`
	system_end=`echo $ENDSTAT | cut -d 'x' -f 3`
	idle_end=`echo $ENDSTAT | cut -d 'x' -f 4`
	iowait_end=`echo $ENDSTAT | cut -d 'x' -f 5`

#echo "[`getDateTime`] RDKB_SELFHEAL : CPU stats after $DELAY sec are"
#echo "user_end: $user_end system_end: $system_end idle_end=$idle_end iowait_end=$iowait_end"
	
	user_diff=$(change 1)
	system_diff=$(change 3)
	idle_diff=$(change 4)
	iowait_diff=$(change 5)

#echo "[`getDateTime`] RDKB_SELFHEAL : CPU stats diff btw 2 intervals is"
#echo "user_diff= $user_diff system_diff=$system_diff and idle_diff=$idle_diff iowait_diff=$iowait_diff"

	active=$(( $user_diff + $system_diff + $iowait_diff))
	total=$(($active + $idle_diff))
	Curr_CPULoad=$(( $active * 100 / $total ))

	echo_t "RDKB_SELFHEAL : CPU usage is $Curr_CPULoad at timestamp $timestamp"
	CPU_THRESHOLD=`syscfg get avg_cpu_threshold`

	count_val=0
	if [ "$Curr_CPULoad" -ge "$CPU_THRESHOLD" ]
	then
		echo_t "RDKB_SELFHEAL : Interrupts"
		echo "`cat /proc/interrupts`"

		echo_t "RDKB_SELFHEAL : Monitoring CPU Load in a 5 minutes window"
	        Curr_CPULoad=0
		# Calculating CPU avg in 5 mins window		
		while [ "$count_val" -lt 10 ]
		do

			count_val=$(($count_val + 1))

			#Record the start statistics
			STARTSTAT=$(getstat)
	
			user_ini=`echo $STARTSTAT | cut -d 'x' -f 1`
			system_ini=`echo $STARTSTAT | cut -d 'x' -f 3`
			idle_ini=`echo $STARTSTAT | cut -d 'x' -f 4`
			iowait_ini=`echo $STARTSTAT | cut -d 'x' -f 5`

			echo_t "RDKB_SELFHEAL : Initial CPU stats are"
			echo "user_ini: $user_ini system_ini: $system_ini idle_ini=$idle_ini iowait_ini=$iowait_ini"

			sleep $DELAY

			#Record the end statistics
			ENDSTAT=$(getstat)

			user_end=`echo $ENDSTAT | cut -d 'x' -f 1`
			system_end=`echo $ENDSTAT | cut -d 'x' -f 3`
			idle_end=`echo $ENDSTAT | cut -d 'x' -f 4`
			iowait_end=`echo $ENDSTAT | cut -d 'x' -f 5`

			echo_t "RDKB_SELFHEAL : CPU stats after $DELAY sec are"
			echo "user_end: $user_end system_end: $system_end idle_end=$idle_end iowait_end=$iowait_end"
	
			user_diff=$(change 1)
			system_diff=$(change 3)
			idle_diff=$(change 4)
			iowait_diff=$(change 5)

			echo_t "RDKB_SELFHEAL : CPU stats diff btw 2 intervals is"
			echo "user_diff= $user_diff system_diff=$system_diff and idle_diff=$idle_diff iowait_diff=$iowait_diff"

			active=$(( $user_diff + $system_diff + $iowait_diff))
			total=$(($active + $idle_diff))
			Curr_CPULoad_calc=$(( $active * 100 / $total ))
			echo_t "RDKB_SELFHEAL : CPU load is $Curr_CPULoad_calc in iteration $count_val"
			Curr_CPULoad=$(($Curr_CPULoad + $Curr_CPULoad_calc))
			
		done

		Curr_CPULoad_Avg=$(( $Curr_CPULoad / 10 ))

		echo_t "RDKB_SELFHEAL : Avg CPU usage after 5 minutes of CPU Avg monitor window is $Curr_CPULoad_Avg"

		if [ ! -f /tmp/CPUUsageReachedMAXThreshold ]
		then
			if [ "$Curr_CPULoad_Avg" -ge "$CPU_THRESHOLD" ];then
				echo "[`getDateTime`] RDKB_SELFHEAL : CPU load is $Curr_CPULoad_Avg"
				echo "[`getDateTime`] RDKB_SELFHEAL : Top 5 tasks running on device"				
				top -bn1 | head -n10 | tail -6
				touch /tmp/CPUUsageReachedMAXThreshold
			fi
		fi

		LOAD_AVG=`cat /proc/loadavg`
		echo_t "RDKB_SELFHEAL : LOAD_AVG is : $LOAD_AVG"

		echo_t "RDKB_SELFHEAL : Interrupts after calculating Avg CPU usage (after 5 minutes)"
		echo "`cat /proc/interrupts`"

#		if [ "$Curr_CPULoad_Avg" -ge "$CPU_THRESHOLD" ]
#		then
#			vendor=`getVendorName`
#			modelName=`getModelName`
#			CMMac=`getCMMac`
#			timestamp=`getDate`
#
#			echo "<$level>CABLEMODEM[$vendor]:<99000005><$timestamp><$CMMac><$modelName> RM CPU threshold reached"
#		
#			threshold_reached=1
#
#			echo "[`getDateTime`] Setting Last reboot reason"
#			reason="CPU_THRESHOLD"
#			rebootCount=1
#			setRebootreason $reason $rebootCount
#
#			rebootNeeded RM CPU
#		fi

####################################################
# Logic : 	If total CPU is 100% and boot time is more than 45 min,
#		Take sum of the cpu consumption of top 5 downstream_manager processes.
#		If total is more than 25%, reboot the box.

		if [ "$BOX_TYPE" = "XB3" ]
                then
			if [ $Curr_CPULoad_Avg -ge $CPU_THRESHOLD ]; then
				bootup_time_sec=`cat /proc/uptime | cut -d'.' -f1`
				if [ $bootup_time_sec -ge 2700 ]; then
					total_ds_cpu=0
					ds_cpu_usage=`top -bn1 | grep downstream_manager | head -n5 | awk -F'%' '{print $2}' | sed -e 's/^[ \t]*//'`
					for each_ds_cpu_usage in $ds_cpu_usage
					do
						total_ds_cpu=`expr $total_ds_cpu + $each_ds_cpu_usage`
					done

					if [ $total_ds_cpu -ge 25 ]; then

						#echo_t "Setting Last reboot reason"
						reason="DS_MANAGER_HIGH_CPU"
						rebootCount=1
						#setRebootreason $reason $rebootCount

						rebootNeeded RM DS_MANAGER_HIGH_CPU $reason $rebootCount
					fi				
				fi
			fi
		fi
fi
####################################################
#Logic:We will read ATOM load average on ARM side using rpcclient, 
#	based on the load average threshold value,reboot the box.
Curr_AtomLoad_Avg=`rpcclient $ATOM_ARPING_IP "cat /proc/loadavg" | sed '4q;d'`
Load_Avg1=`echo $Curr_AtomLoad_Avg | awk  '{print $1}'`
Load_Avg10=`echo $Curr_AtomLoad_Avg | awk  '{print $2}'`
Load_Avg15=`echo $Curr_AtomLoad_Avg | awk  '{print $3}'`
        if [ ${Load_Avg1%%.*} -ge 5 ] && [ ${Load_Avg10%%.*} -ge 5 ] && [ ${Load_Avg15%%.*} -ge 5 ]; then
		#echo_t "Setting Last reboot reason as ATOM_HIGH_LOADAVG"
		reason="ATOM_HIGH_LOADAVG"
		rebootCount=1
		#setRebootreason $reason $rebootCount
		rebootNeeded RM ATOM_HIGH_LOADAVG $reason $rebootCount
	fi

####################################################
	
#	sh $TAD_PATH/task_health_monitor.sh
	if [ -f  /usr/bin/Selfhealutil ]
  	then
  		Selfhealutil power_mode
  		batteryMode=$?
  	        echo_t "RDKB_SELFHEAL : batteryMode is  $batteryMode"
    	fi	
     	   
  	if [ $batteryMode = 0 ]
  	then
	     sh $TAD_PATH/task_health_monitor.sh
	fi

	SELFHEAL_ENABLE=`syscfg get selfheal_enable`
		
done
