for v in 512 1024 #2048 4096 8192 16384
do
  for d in 4 8 #16 32 128 256
  do
    e=`expr $v \* $d`
    f="v${v}_e${e}.txt"
    echo ""
    echo "./gpu_floyd ../data/$f output/$f"
    time ./gpu_floyd ../data/$f output/$f
  done
done
