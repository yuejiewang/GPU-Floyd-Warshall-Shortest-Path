dir="/scratch/yw4989/GPU"
for v in 512 1024 2048 4096
do
  for d in 4 8 16 128
  do
    e=`expr $v \* $d`
    f="v${v}_e${e}.txt"
    echo ""
    echo "./seq_floyd ${dir}/data/$f ${dir}/seq/$f"
    time ./seq_floyd ${dir}/data/$f ${dir}/seq/$f
  done
done
