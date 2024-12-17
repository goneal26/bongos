# BongOS -> A 64-bit hobby operating system

![Temporary Logo](bongos_logo.png)

Pronounced like the drum or the drug apparatus, whichever you prefer.

Originally built from the Limine Bootloader template in C, this project has been
rewritten in Rust, following along with the 
[Writing an OS in Rust](https://os.phil-opp.com/) tutorial. 

The project's codebase diverges from 
[this point](https://github.com/phil-opp/blog_os/tree/post-02) in the tutorial's
codebase.

## Roadmap

- [x] Freestanding Rust binary and build system
- [x] VGA text mode interfacing
- [x] Basic font rendering
- [ ] Basic text buffer display + formatted printing
- [ ] Getting and displaying hardware info (memory layout, tables, etc)
- [ ] Interrupt handling and keyboard input

## Building

This project requires a nightly version of Rust because it uses some unstable 
features. At least nightly _2020-07-15_ is required for building. You might need 
to run `rustup update nightly --force` to update to the latest nightly even if 
some components such as `rustfmt` are missing it.

You can build the project by running:

```
cargo build
```

To create a bootable disk image from the compiled kernel, you need to install 
the [`bootimage`] tool:

[`bootimage`]: https://github.com/rust-osdev/bootimage

```
cargo install bootimage
```

After installing, you can create the bootable disk image by running:

```
cargo bootimage
```

This creates a bootable disk image in the `target/x86_64-bongos/debug` 
directory.

## Running

You can run the disk image in [QEMU] through:

[QEMU]: https://www.qemu.org/

```
cargo run
```

[QEMU] and the [`bootimage`] tool need to be installed for this.

You can also write the image to an USB stick for booting it on a real machine. 
On Linux, the command for this is:

```
dd if=target/x86_64-bongos/debug/bootimage-bongos.bin of=/dev/sdX && sync
```

Where `sdX` is the device name of your USB stick. **Be careful** to choose the 
correct device name, because everything on that device is overwritten.
