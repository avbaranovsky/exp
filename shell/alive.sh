#/bin/sh
is_alive=`ping -c 1 -q ${1} | grep received | cut -d ' ' -f 2`

echo $is_alive

if [ $is_alive -eq 1 ]
then
	echo ${1} is up
else
	echo ${1} is down
fi
