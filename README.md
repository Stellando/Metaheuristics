
GNUPLOT指令:
set title "Hill Climbing Performance"
set xlabel "Iteration"
set ylabel "Best Value"
set grid
plot "result_Hill1.txt" using 1:2 with lines lw 2 lc rgb "blue" title "Run 1"
