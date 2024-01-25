#! /usr/bin/env bash
#exec 3<$1
#exec 4<$2
#touch $3
#touch $4

#echo "res">$3
#echo "error">$4
total_time=0
space_cost=0
error_count=0
strs=''
function randStr
{
	#for i in {a..z};do array[$j]=$i;j=$(($j+1));done
	#for i in {A..Z};do array[$j]=$i;j=$(($j+1));done
	for ((i=0;i<$1;i++))
	do strs="${strs}a"
	done
}
row1=$1
for((j=0;j<10;j++))
do
	if [ $j -lt 5 ];then
		randStr 200
	elif [ $j -lt 7 ];then
		randStr 500
	else
		randStr 1000
	fi
	row2=$strs
	if ((${#row2} % 3 == 0)); then	
		row2="${row2}aa"
	fi
	out=`./match $row1 $row2`
	if [ "$?" != 0 ]
	then
		error_count=$(($error_count+1))
		#echo $row1 >>$4
		#echo $row2 >>$4
	fi
	#echo $out >>$3
	echo ${array[1]}
	echo ${array[2]}
	echo ${array[3]}
	array=($out)
	res=${array[0]}
	#total_time=$(($total_time+${array[1]}))
	total_time=$(echo $total_time ${array[3]} | awk '{printf("%.10f"),$1+$2}' )
	space_cost=$(echo $space_cost ${array[4]} | awk '{printf("%.10f"),$1+$2}' )
	#echo $row2
	echo "length: ${#strs}"
	echo "total_time : $total_time"
	echo "space_cost : $space_cost"
	total_time=0
	space_cost=0
done
