for v in 512 1024 #2048 4096 8192 16384
do
  for d in 4 8 #16 32 128 256
  do
    e=`expr $v \* $d`
    echo "python3 g.py $v $e"
    python3 g.py $v $e
  done
done
