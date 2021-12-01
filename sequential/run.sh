dir="/scratch/yw4989/GPU"
for v in 8192
do
  for d in 16 32
  do
    e=`expr $v \* $d`
    f="v${v}_e${e}.txt"
    echo ""
    echo "./seq_floyd ${dir}/data/$f ${dir}/seq/$f"
    time ./seq_floyd ${dir}/data/$f ${dir}/seq/$f
  done
done
