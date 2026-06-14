#!/bin/sh
rc=0

calc_dir()
{
  echo 'processing '`pwd`
  list=`ls`
  checksumfile=$1/`pwd|sed 's/\//_/g'`'.md5'
  md5sum -c $checksumfile >> $1/check.log 
  retcode=$?
  if [ $retcode -ne 0 ]
  then
    rc=1
  fi

  lsfile=$1/`pwd|sed 's/\//_/g'`'.ls'
  lstfile=$1/`pwd|sed 's/\//_/g'`'.lst'
  ls -l > $lstfile
  diff -q $lstfile $lsfile >> $1/check.log 

  retcode=$?
  if [ $retcode -ne 0 ]
  then
    rc=1
  else
    rm $lstfile
  fi

  for item in $list
  do
    if [ -d $item ]
    then
      cd $item
      calc_dir $1 
      cd ..
    fi
  done
}

initdir=`pwd`
rm $initdir/check.log
cd $1

calc_dir $initdir 

if [ $rc -ne 0 ]
then
 echo ==== FAILED: =====
 cat $initdir/check.log
else
 echo ==== SUCCESS =====
fi
cd $initdir


