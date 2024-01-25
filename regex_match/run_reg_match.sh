#! /usr/bin/env bash
exec 3<$1
exec 4<$2
touch $3
touch $4
echo "error" >$4
echo "log" >$3
FA_time=0
match_time=0
total_time=0
space_cost=0
while read -r row1<&3 && read -r row2<&4
do
	#$row1=$row1 | tr -d "\r"
	#$row2=$row2 | tr -d "\r"
	#echo  $row1
	#echo $row2
	row2=${row2%?}
	row1=${row1%?}
	#echo $row1
	#echo $row2
	out=`./match $row1 $row2 2>>$4`
	echo $out >>$3
	array=($out)
	res=${array[0]}
	FA_time=$(echo $FA_time ${array[1]} | awk '{printf("%.10f"),$1+$2}' )
	match_time=$(echo $match_time ${array[2]} | awk '{printf("%.10f"),$1+$2}' )
	total_time=$(echo $total_time ${array[3]} | awk '{printf("%.10f"),$1+$2}' )
	space_cost=$(echo $space_cost ${array[4]} | awk '{printf("%.10f"),$1+$2}' )
done
echo "FA_time: $FA_time"
echo "match_time: $match_time"
echo "total_time: $total_time"
echo "space_cost: $space_cost"
