<!-- Logo -->
<p align="center">
    <a href="https://github.com/TheBigEye/Monarch-OS#gh-light-mode-only"> <!-- if light mode -->
        <img width="42%" src="https://github.com/TheBigEye/TheBigEye/blob/main/Projects/Monarch-OS/Light-header.svg?raw=true" alt="Light mode Monarch OS logo!"/>
    </a>
    <a href="https://github.com/TheBigEye/Monarch-OS#gh-dark-mode-only"> <!-- if dark mode -->
        <img width="42%" src="https://github.com/TheBigEye/TheBigEye/blob/main/Projects/Monarch-OS/Dark-header.svg?raw=true" alt="Dark mode Monarch OS logo!!"/>
    </a>
</p>

<!-- Badges -->
<p align="center">
     <a href="https://github.com/TheBigEye#gh-light-mode-only"> <!-- if light mode -->
          <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=FF9641&color=4f4f4f" title="Made with C++"/>
          <img src="https://komarev.com/ghpvc/?username=Eye-Monarch-OS&label=Views&color=FF9641&style=for-the-badge" title="Views" />
          <img src="https://img.shields.io/badge/VSCode-0078D4?style=for-the-badge&logo=visual%20studio%20code&logoColor=FF9641&color=4f4f4f" title="Programmed using VScode"/>
     </a>
     <a href="https://github.com/TheBigEye#gh-dark-mode-only"> <!-- if dark mode -->
          <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=FF9641&color=4f4f4f" title="Made with C++"/>
          <img src="https://komarev.com/ghpvc/?username=Eye-Monarch-OS&label=Views&color=000000&style=for-the-badge" title="Views"/>
          <img src="https://img.shields.io/badge/VSCode-0078D4?style=for-the-badge&logo=visual%20studio%20code&logoColor=FF9641&color=000000" title="Programmed using VScode"/>
     </a>
</p>

**Monarch OS** is an Operating System based on 64 bits in VGA text mode, it is composed of a Kernel (as the main program) and a Bootloader (reads the disk, searches and loads the kernel in memory), it also contains a Memory manager and Commands interpreter

It has been created from scratch using several videos from Youtube as a base, and https://wiki.osdev.org, with the aim of trying to make a real OS, currently still under development :)

## How do I build this?
To build, you will need NASM, GCC and a 64 bit processor, optionally you can use a Linux environment such as Ubuntu or Debian, (although the BATCH scripts should be re-adapted to BASH or Make), alternatively you can use WSL or just MSYS2 on Windows.

### MSYS2 (Windows):
- Download and install [**MSYS2**](https://www.msys2.org/), once installed and updated, you will need to install some packages under the MINGW64 shell, `pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-nasm`, once the packages are installed, you must add `C:\msys64\mingw64\bin` to the Windows PATH so that it can be used externally, for that, you open the taskbar search box and type `env`, you should see the option to edit system environment variables, in the pop-up window you click `Environment variables`, in the next window you select `Path` of System section, and there you add that path to the bin folder, now typing `gcc --version` in CMD should work :)
- And... what happens if I want to use a cross-compiler like ELF?, To avoid complications, download the zip corresponding to your architecture and system from [**here**](https://github.com/lordmilko/i686-elf-tools/releases/tag/7.1.0) (remember x86_64 is 64 bit, and i686 is 32 bit, in this case we are using 64 bit), once downloaded, go to the following path `C:\msys64\mingw64`, drag the zip folders into the mingw64 folder, and that will be it, you can try running `x86_64-elf-gcc --version` from CMD to check if works.

Once the compilers and tools are configured, run the **build.bat** script, it should generate an floppy image on the Build folder.

## How do I run this?
Currently for now Virtual Box or VMware will not work because the generated floppy file does not have the proper format, to run the system it is necessary to use QEMU or Bochs

<!-- -------------------------------------------------------------------------- Credits ------------------------------------------------------------------------------>
<!-- Header and footer svgs --- kyechan99/capsule-render -->
<!-- Views counter --- antonkomarev/github-profile-views-counter -->
<!-- ---------------------------------------------------------------------------- END -------------------------------------------------------------------------------->
