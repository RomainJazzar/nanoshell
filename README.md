# Nanoshell

A simple Unix shell implementation in C.

## Description

Nanoshell is a minimalist shell that can:

- Display a prompt `?>`.
- Run commands with absolute paths or by searching the `PATH`.
- Handle arguments.
- Execute built-ins: `cd`, `exit`, `pwd`, `env`.
- Report errors in a specific format.

## Installation & Usage

1. **Compile:**
   Run `make` in the terminal.
   ```bash
   make
   ```

# Nanoshell

A simple, custom Unix shell implementation in C.
This shell can execute commands, handle paths, and run built-in functions like `cd` and `exit`.

---

## 🛠️ Prerequisites (Read this first!)

### If you are on Windows (New PC)

You cannot run C code directly on Windows. You must use **WSL** (Linux Subsystem).

1.  **Install WSL:**
    Open PowerShell as Administrator and run:

    ```powershell
    wsl --install
    ```

    _Restart your computer if asked._

2.  **Open Linux:**
    Open the "Ubuntu" app or type `wsl` in PowerShell.

3.  **Install Compiler Tools:**
    Inside the Linux terminal, run this command to get `gcc` and `make`:
    ```bash
    sudo apt update && sudo apt install build-essential -y
    ```

### If you are on Mac / Linux

You just need the compiler tools.

- **Mac:** `xcode-select --install`
- **Linux:** `sudo apt install build-essential`

---

## 🚀 How to Run

1.  **Navigate to the folder:**
    (If you are on Windows using WSL, remember your files are in `/mnt/c/Users/...`)

    ```bash
    cd /path/to/your/folder
    ```

2.  **Compile the code:**
    This creates the executable file.

    ```bash
    make
    ```

3.  **Launch the Shell:**
    ```bash
    ./nanoshell
    ```

---

## 🎮 Usage

Once inside the shell (you will see the prompt `?>`), you can run commands:

### Built-in Commands

- **`cd <folder>`** : Change directory.
- **`cd ..`** : Go back one folder.
- **`cd ~`** : Go to home directory.
- **`pwd`** : Show current directory path.
- **`env`** : Show environment variables.
- **`exit`** : Close the shell.

### System Commands

You can run any standard Linux command:

- `ls -la`
- `mkdir new_folder`
- `cat Makefile`
- `grep "hello" file.txt`

---

## 🧹 Maintenance

To clean up the compiled files (executable and object files) before submitting or uploading to GitHub:

```bash
make clean
```
