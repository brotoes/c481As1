file = 'average.dat'

set grid
set datafile missing '@'

set autoscale

stats file u 1 every 4::3 name 'A'
stats file u 1 every 4::2 name 'B'
stats file u 1 every 4::1 name 'C'
stats file u 1 every 4::0 name 'D'

set term png

set arrow from 4,1 to 4,5 nohead
set label "Hyperthreading" at 2.2,4.5
set output "speedup.png"
set xlabel '# of Cores'
set ylabel 'Speedup'
set title 'Speedup'
set xrange [1:8]
set yrange [1:5]
plot file u 5:(A_max/$1) every 4::3 ti '4096x4096' w lp, \
'' u 5:(B_max/$1) every 4::2 ti '2048x2048' w lp, \
'' u 5:(C_max/$1) every 4::1 ti '1024x1024' w lp, \
x ti "Linear Speedup"
#'' u 5:(D_max/$1) every 4::0 ti '512x512' w lp, \

unset label
unset arrow
set arrow from 4,0 to 4,10 nohead
set label "Hyperthreading" at 2.2,70
set output "time.png"
set xlabel '# of Cores'
set ylabel 'Time (log(s))'
set title 'Time'
set xrange [1:8]
set yrange [0:10]
plot file u 5:(log($1)) every 4::3 ti '4096x4096' w lp, \
'' u 5:(log($1)) every 4::2 ti '2048x2048' w lp, \
'' u 5:(log($1)) every 4::1 ti '1024x1024' w lp
#'' u 5:(log($1)) every 4::0 ti '512x512' w lp

unset arrow
unset label
set title "Time per Segment (1024x1024)"
set ylabel "Time (log(μs))"
set output "segment.png"
set style data histogram
set style histogram cluster gap 2
set style fill solid border rgb "black"
set auto x
set yrange [0:20]
plot file u (log($2*1000000)):xtic(5) every 4::1 ti "Initialize", \
'' u (log($3*1000000)):xtic(5) every 4::1 ti "Generate", \
'' u (log($4*1000000)):xtic(5) every 4::1 ti "Multiply"
