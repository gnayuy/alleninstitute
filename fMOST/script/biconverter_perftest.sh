# j=4; n=6550; for((i=0; i<$j; i++)); do ln -s /local4/data/slice256/test_0$((n++))_mon.tif slice$j/; done

display_time_memuse()
{
  date '+%Y-%m-%d %H:%M:%S'
  free -m | awk 'NR==2{printf "Memory Usage: %s/%sMB (%.2f%%)\n", $3,$2,$3*100/$2 }'
}

display_time_memuse
time sh test2.sh slice4 conv2slice4

display_time_memuse
time sh test2.sh slice8 conv2slice8

display_time_memuse
time sh test2.sh slice16 conv2slice16

display_time_memuse
time sh test2.sh slice32 conv2slice32

display_time_memuse
time sh test2.sh slice64 conv2slice64

display_time_memuse
time sh test2.sh slice128 conv2slice128

display_time_memuse
time sh test2.sh slice256 conv2slice256


