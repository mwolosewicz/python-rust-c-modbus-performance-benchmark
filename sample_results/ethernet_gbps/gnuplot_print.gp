set terminal png size 1920,1080 # Set the output format to PNG and the size of the image
set output 'benchmark_comparison.png' # Set the output file name

set title "Modbus Response Time Benchmark - eth 1Gbps (1000 reads of 10 registers) - 1000 measurements" # Title of the chart
set xlabel "Measurement Number" # Label for the x-axis
set ylabel "Response Time (ms)" # Label for the y-axis

set key outside # Place the legend (key) outside of the plot
set grid # Enable grid

# Plot the data
plot 'c_data_eth.txt' using 1 with lines title 'C', \
     'rust_data_eth.txt' using 1 with lines title 'Rust', \
     'python_data_eth.txt' using 1 with lines title 'Python'
