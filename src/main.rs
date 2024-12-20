#![no_std]
#![no_main]
#![feature(custom_test_frameworks)]
#![test_runner(bongos::test_runner)]
#![reexport_test_harness_main = "test_main"]

use bongos::println;
use core::panic::PanicInfo;

#[no_mangle]
pub extern "C" fn _start() -> ! {
  println!("Hello world!");

  #[cfg(test)]
  test_main();

  loop {} // hang
}

// called on panic
#[cfg(not(test))]
#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
  println!("{}", info);
  loop {}
}

// panic handler in test mode
#[cfg(test)]
#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
  bongos::test_panic_handler(info)
}
