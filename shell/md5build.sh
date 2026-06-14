#!/bin/sh

calc_dir()
{
  echo 'processing '`pwd`
  list=`ls`
  checksumfile=$1/`pwd|sed 's/\//_/g'`'.md5'
  md5sum -b * > $checksumfile

  lsfile=$1/`pwd|sed 's/\//_/g'`'.ls'
  ls -l > $lsfile

  for item in $list
  do
    if [ -d $item ]
    then
      cd $item
      calc_dir $1
      cd ..
    fi
  done
  echo done
}


echo list=$list
echo checksumfile=$checksumfile
initdir=`pwd`

cd $1
calc_dir $initdir
cd $initdir

