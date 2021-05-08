# Instructions for setting up toolchain using Windows 10, Windows Subsystem for Linux (wsl2) and Ubuntu 20.04 LTS

This is a work in progress

### Step 1: Set up and configure Windows Subsystem for Linux v2 (wsl2) on Windows 10

Here is a detailed explanation [link to wsl2 article] (https://code.visualstudio.com/blogs/2019/09/03/wsl2) of what to do and check
You will then need to download Ubuntu 20.02 LTS from the windows store

So you can get into a bash shell for Ubuntu in various ways, but I just click on start menu and start typing Ubuntu ...

You should get a bash shell.

### Step 2: Update Ubuntu

```
sudo apt clean
sudo apt update
sudo apt upgrade
```

### Step 3: Install Visual Studio Code for Windows (VS Code)

Here we install VS Code as a normal windows app, not a Linux app - that doesn't work because wsl2 does not yet support GUIs (afaik) (Well I initially tried and it didn't work.)

### Step 4: Install the VS Code WSL extension

VS Code - Extensions: search Remote WSL and install - you may need to reboot VS Code (CTRL SHIFT P and Developer Reload Window or perhaps just Reload Window (I'm using an Insiders version of VS Code)

This is where the magic happens. This extension allows Vs Code to access the Linux subsystem rather than the windows workspace (if that's the right word?)

If you look at the very bottom left you will see a green icon


If you click on it you will get a popup window, click on "new WSL window" or try the other options :)
You don't need to keep the original VS Code is open. It will remember next time you open VS Code. To go back to Windows workspace click on green icon and select as appropriate.

Note how the green icon at the bottom left changes to "WSL:Ubuntu:20.04"
VS Code is now looking at the Linux workspace.

### Step 5: Install VS Code extensions

Install C/C++ Extension Pack - accept the message which installs Linux stuff
Install Cortex-debug in VSCode
Install Task manager extension (Zhang Shen??) (This is cool. It allows shortcuts to clean, build and other things you want to do - the json is in .vscode > task.json if you want to add your own.


### Step 6: Install Linux pre-requisites as per documentation

[link to documentation](development-flash_and_debug_firmware.md)

But in brief:

```
sudo usermod -a -G dialout USER_NAME so for me sudo usermod -a -G dialout gordon

sudo apt-get install openocd
sudo apt-get install gdb-multiarch
sudo apt-get install gcc-arm-none-eabi
sudo apt-get install binutils-arm-none-eabi
sudo apt-get install libnewlib-arm-none-eabi
sudo apt-get install srecord

```

Note: I already had a toolchain installed for compiling C programs, that is gcc etc, but I can't remember what is needed. You may need to do this. Please let us know the details so it can be added to this document.

(I think it would be good to close down stuff and do a total reboot - it's upto you ;) )

Nearly there.

### Step 7: Clone a repo

For me I created a directory (folder = windows nomenclature!) called ebike.

Now in VS Code if you click on the folders/directory icon at top left of the vertical bar (it's got a ui name but I forget - stack I think?) it will (should) show an option to open a folder or clone a repository. Choose clone and enter the location of the github repo you want.

Select the ebike directory and the repo will be cloned in a folder underneath this location.

https://github.com/OpenSourceEBike/TSDZ2_wireless

(This also contains the EBike_wireless_remote code, though there is also a higher level ebike_wireless_remote - I'm awaiting direction on which to use.)

### Step 8: Open the workspace you require

(This was new to me, but the repo TSDZ2_wireless uses a file called workspace.code-workspace that organises the projects - at least that's how I understand it. Opening a workspace allows you to open the project you are interested in (more easily perhaps?) )

[link to article on VS Code workspace](https://code.visualstudio.com/docs/editor/workspaces)

### Step 9: Clean and build

You can now use the icon on the left stack (probably at the bottom) (or in the hidden items indicated by ...) called Task Manager.

From the Task Manager select clean and the run icon, the build and the run icon.

Hopefully! the project you opened compiles - though I'm currently having a problem with the EBike_Wirelss_TSDZ2 repo.

### Step 10: What else?

### Step 99: Do something cool!