


A Linux-inspired Virtual File System written in C that simulates inode-based file management similar to Unix file systems.

# 📂 Custom Virtual File System (CVFS)

A **Custom Virtual File System (CVFS)** implemented in **C Programming** that simulates the internal working of a Linux file system.  
This project demonstrates how operating systems manage files using **inodes, file descriptors, and memory management**.

---

# 📌 Project Overview

The **CVFS project** is a command-line based system programming project that replicates file system behavior such as:

- File Creation
- File Opening
- File Reading
- File Writing
- File Deletion
- File Information Display
- Command Manual System

The entire system runs **in RAM using dynamic memory allocation**.

---

# 🎯 Objectives

- Understand **Linux File System Architecture**
- Implement **inode-based file management**
- Simulate **file operations**
- Learn **System Programming Concepts**
- Strengthen **Data Structures knowledge**

---

# ⚙️ Technology Used

|           Technology         |           Purpose          |
|------------------------------|----------------------------|
| C Programming                | Core implementation        |
| Data Structures              | Inode and file management  |
| Dynamic Memory Allocation    | File storage               |
| GCC Compiler                 | Compilation                |
| CLI (Command Line Interface) | User interaction           |

---

# 🖥️ User Interface

The system uses **Command Line Interface (CLI)**.

Example commands:

```bash
create file1 3
open file1 3
write file1
read file1 10
ls
unlink file1
````

---

# 🧱 System Architecture

The CVFS system is composed of the following components:

```
User Commands
      |
      v
+-------------------+
|   Command Parser  |
+-------------------+
          |
          v
+-------------------+
|       UFDT        |
| (File Descriptor) |
+-------------------+
          |
          v
+-------------------+
|     File Table    |
+-------------------+
          |
          v
+-------------------+
|       Inode       |
+-------------------+
          |
          v
+-------------------+
|    Data Blocks    |
+-------------------+
```

---

# 🗂 Data Structures Used

### Superblock

Tracks total and free inodes.

```c
struct SUPERBLOCK
{
    int TotalInodes;
    int FreeInodes;
};
```

---

### Inode

Represents file metadata.

```c
struct inode
{
    char FileName[50];
    int InodeNumber;
    int FileSize;
    int FileActualSize;
    int FileType;
    char *Buffer;
    int LinkCount;
    int ReferenceCount;
    int Permission;
    struct inode *next;
};
```

---

### File Table

Stores information of opened files.

```c
struct filetable
{
    int ReadOffset;
    int WriteOffset;
    int Count;
    int Mode;
    struct inode *ptrinode;
};
```

---

### UFDT (User File Descriptor Table)

Maps file descriptors to file tables.

```c
struct ufdt
{
    struct filetable *ptrfiletable;
};
```

---

# 📜 Supported Commands

| Command                      | Description                |
| ---------------------------- | -------------------------- |
| `create <name> <permission>` | Create new file            |
| `open <name> <mode>`         | Open file                  |
| `read <name> <size>`         | Read file                  |
| `write <name>`               | Write into file            |
| `close <name>`               | Close file                 |
| `ls`                         | List all files             |
| `unlink <name>`              | Delete file                |
| `man <command>`              | Display command manual     |
| `help`                       | Display all commands       |
| `exit`                       | Exit CVFS                  |

---

# 🔄 Project Flow

```
Start Program
      |
Initialize Superblock
      |
Create Inode Linked List
      |
Accept User Command
      |
Parse Command
      |
Execute Function
      |
Display Output
      |
Repeat Until Exit
```

---

# ▶️ How to Run the Project

### Step 1: Compile

```bash
gcc cvfs.c -o cvfs
```

### Step 2: Run

```bash
./cvfs
```

---

# 💻 Example Output

```
CVFS : > create file1 3
File created successfully with file descriptor : 0

CVFS : > write file1
Enter data : Hello World

CVFS : > read file1 11
Hello World

CVFS : > ls
file1

CVFS : > unlink file1
File deleted successfully
```

---

# 📸 Output Screenshots

### File Creation

```
CVFS > create file1 3
File created successfully
```

### File Reading

```
CVFS > read file1 11
Hello World
```

### Listing Files

```
CVFS > ls
file1
```

### File Deletion

```
CVFS > unlink file1
File deleted successfully
```

---

# ⚠️ Limitations

* Data stored only in RAM
* No directory structure
* Limited inode count
* No multi-user support

---

# 🚀 Future Enhancements

* Persistent storage
* Directory support
* Multi-user system
* File encryption
* GUI interface

---

# 📚 Learning Outcomes

This project helps understand:

* File System Architecture
* Inode-based File Management
* File Descriptors
* Memory Management
* System Programming

---

# 👨‍💻 Author

**Aashish Vilas Labade**

* 📍 Pune, Maharashtra
* 💻 C / C++ / Java Developer
* 🔗 GitHub: https://github.com/Aashish080504

---

⭐ If you like this project, give it a **star on GitHub!**

```
