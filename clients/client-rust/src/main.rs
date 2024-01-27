extern crate tokio_modbus;

use tokio_modbus::prelude::*;
use tokio_modbus::client::Context;
use tokio::time::Instant;
use std::env;
use std::error::Error;
use core::time::Duration;

async fn perform_readings(ctx: &mut Context, times: u32) -> Result<Duration, Box<dyn std::error::Error>> {
    let start_time = Instant::now();
    for _ in 0..times {
        let _ = ctx.read_holding_registers(0, 10).await?;
    }
    Ok(start_time.elapsed())
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn Error>> {
    let args: Vec<String> = env::args().collect();

    if args.len() < 5 {
        eprintln!("Usage: {} <ip_address> <port> <times> <number_of_tests>", args[0]);
        std::process::exit(1);
    }

    let ip_address = &args[1];
    let port = &args[2];
    let times: u32 = args[3].parse().expect("Invalid number for 'times'");
    let number_of_tests: u32 = args[4].parse().expect("Invalid number for 'number_of_tests'");

    let addr = format!("{}:{}", ip_address, port).parse()?;
    let mut ctx = tcp::connect(addr).await?;

    let mut cumulative_duration: Duration = Default::default();

    for _ in 0..number_of_tests {
        let duration = perform_readings(&mut ctx, times).await?;
        println!("Time for making {} readings: {} ms", times, duration.as_millis());
        cumulative_duration += duration;
    }

    println!("Average time for making {} readings: {:?} ms", times, (cumulative_duration/number_of_tests).as_millis());

    Ok(())
}