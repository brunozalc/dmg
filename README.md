# dmg

A GameBoy (DMG) emulator written in C.

The final goal is to have a fully functional program that can faithfully emulate the Gameboy hardware and its games, specially commercial titles. The emulator is currently in development.

Want to read more about the project and my learning experience? Check out my personal website and (WIP) blog: [zalcberg.me](https://zalcberg.me).

---

## Roadmap

| Stage | Description                                                 | Status |
| ----- | ----------------------------------------------------------- | ------ |
| 0     | Project setup                                               | ✅     |
| 1     | Core emulator w/ CPU emulation and complete instruction set | ✅     |
| 2     | PPU emulation (part 1)                                      | ⏳     |
| 3     | Joypad + bank switching                                     | ❌     |
| 4     | PPU emulation (part 2)                                      | ❌     |
| 5     | APU and QOL improvements                                    | ❌     |
| 6     | Multiplatform support                                       | ❌     |

After that, the plan is to add support for modern controllers such as the Dualsense and Xbox joypads. Eeventually, GBC support should be added as well.

---

## Building and running locally

Currently, the emulator is only supported on MacOS, which is my development environment. The emulator is built using Makefile, so you can build it by running the following command in the root directory of the project:

```bash
make
```

This will create a binary file called `gb` in the root directory of the project. You can run the emulator by executing the following command:

```bash
./gb <path_to_rom>
```
