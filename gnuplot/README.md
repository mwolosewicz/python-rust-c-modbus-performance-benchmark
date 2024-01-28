# Print charts.

To print chart, you can use the `gnuplot_print.gp` config.
Feel free to modify the script.

To get data for visualization you can redirect data stream to file like this:
```bash
./c_modbus_client 127.0.0.1 1502 1000 1000 > c_data.txt
```

Do the same for other languages as well.

Then to generate chart from files with measurements, you can use:
```bash
gnuplot gnuplot_print.gp
```
