#!/bin/bash
rake clean
sstat_dir="$PWD/ext/sstat"
lib_dir="$PWD/lib"
root_folder=$PWD
echo $sstat_dir
cd $sstat_dir
`ruby extconf.rb --enable-static`
make
cp "${sstat_dir}/sstat.o" $lib_dir
cd $PWD