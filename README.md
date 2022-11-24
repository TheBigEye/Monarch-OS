<!-- Logo -->
<p align="center">
    <a href="https://github.com/TheBigEye/Monarch-OS#gh-light-mode-only"> <!-- light mode -->
        <img
            width="42%"
            src="https://github.com/TheBigEye/TheBigEye/blob/main/Projects/Monarch-OS/Light-header.svg?raw=true" alt="Light mode Monarch OS logo!"
        />
    </a>
    <a href="https://github.com/TheBigEye/Monarch-OS#gh-dark-mode-only"> <!-- dark mode -->
        <img
            width="42%"
            src="https://github.com/TheBigEye/TheBigEye/blob/main/Projects/Monarch-OS/Dark-header.svg?raw=true" alt="Dark mode Monarch OS logo!!"
        />
    </a>
</p>

<!-- Badges -->
<p align="center"> 
     <a href="https://github.com/TheBigEye#gh-light-mode-only"> <!-- light mode -->
          <img 
               src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=FF9641&color=4f4f4f"
               Title="Made with C++"  
          />
          <img 
               src="https://komarev.com/ghpvc/?username=Eye-Monarch-OS&label=Views&color=FF9641&style=for-the-badge" 
               Title="Views" 
          />
          <img 
               src="https://img.shields.io/badge/VSCode-0078D4?style=for-the-badge&logo=visual%20studio%20code&logoColor=FF9641&color=4f4f4f" 
               Title="Programmed using VScode" 
          />
     </a>
     <a href="https://github.com/TheBigEye#gh-dark-mode-only"> <!-- dark mode -->
          <img 
               src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=FF9641&color=4f4f4f" 
               Title="Made with C++"  
          />
          <img 
               src="https://komarev.com/ghpvc/?username=Eye-Monarch-OS&label=Views&color=000000&style=for-the-badge" 
               Title="Views" 
          />
          <img 
               src="https://img.shields.io/badge/VSCode-0078D4?style=for-the-badge&logo=visual%20studio%20code&logoColor=FF9641&color=000000" 
               Title="Programmed using VScode" 
          />
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

<!-- ## Features

#### Desktop environment:
- It is composed of a wallpaper along with a taskbar.
- The taskbar contains a clock, volume and internet icons.
- A start menu.
- Draggabble windows.

#### Programs:
- **Terminal** - you can write and execute commands.
- **File manager** - You can see files and folders, change directories and see the size of each file.
- **Browser** - You can browse the internet (currently it only uses html 3, so most of the pages don't work very well).

#### Miscellaneous:
- **Boot loader** (animation).
- **BIOS**.
- **RSOD** (Red Screen of Death).
- **BSOD** (Black Screen of Death).
- **GSOD** (Green Screen of death).
- **Login** (unused).
- **Desktop mode**.
- **Terminal mode.**
- **File system**.

#### Some current ideas:
- **Antivirus (fake).**
- **Control pane.**
- **Draggable desktop icons.**
- **Internet.**
- **System configuration.**
- **Reboot and shutdown.**

## Bugs
- Dragging windows can cause their content to flicker or no longer render.
- Sometimes taskbar buttons flicker.

## Installation
Download the code and unzip it, it is necessary to have Python 3.10 and have the following modules installed 

- [`Pillow`          ](https://github.com/python-pillow/Pillow)          Necessary so that the program can read the images and work.
- [`Psutil`          ](https://github.com/giampaolo/psutil)              Necessary for the neofetch command and some functions to work.
- [`Tkinterweb`      ](https://github.com/Andereoo/TkinterWeb)           Necessary for the browser to work.
- [`TkinterMapView`  ](https://github.com/TomSchimansky/TkinterMapView)  Necessary for the Map Viewer to work.

For a quick installation use the following command inside the project folder:

```sh
pip install -r requirements.txt
```

And double click on OS.py file to run the project or just in the command line you write 
```sh
python OS.py
```
**NOTE**: If you're on a on a minimal Linux installation, you need a desktop or window manager compatible with the `python-tk` package to work ._.
-->
#

<!-- -------------------------------------------------------------------------- Credits ------------------------------------------------------------------------------>
<!-- Header and footer svgs --- kyechan99/capsule-render -->
<!-- Views counter --- antonkomarev/github-profile-views-counter -->
<!-- ---------------------------------------------------------------------------- END -------------------------------------------------------------------------------->
