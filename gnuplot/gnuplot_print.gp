set terminal png size 1920,1080 # Set the output format to PNG and the size of the image
set output 'benchmark_comparison.png' # Set the output file name

set title "Modbus Response Time Benchmark" # Title of the chart
set xlabel "Measurement Number" # Label for the x-axis
set ylabel "Response Time (ms)" # Label for the y-axis

set key outside # Place the legend (key) outside of the plot
set grid # Enable grid

# Plot the data
plot 'c_data.txt' using 1 with lines title 'C', \
     'rust_data.txt' using 1 with lines title 'Rust', \
     'python_data.txt' using 1 with lines title 'Python'
