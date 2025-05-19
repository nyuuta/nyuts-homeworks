🌟 MyFS — A Simple In-Memory File System

MyFS is a lightweight, in-memory file system built with FUSE. It simulates a basic file system structure and behavior, storing all data temporarily in RAM during execution.

-------------------------

## ✨ Key Features

* 📄 Create, read, and write files
* 📂 List directory contents
* 🕒 Track file metadata (size, timestamps, permissions)
* 💾 Simulated disk using fixed-size memory blocks

-------------------------

## 🧰 Requirements

* Linux (Ubuntu recommended)
* C++17-compatible compiler (g++)
* FUSE 3 development libraries (libfuse3-dev)
* make utility

Install required dependencies:

chmod +x install_deps.sh
./install_deps.sh

-------------------------

## ⚙️ Building the Project

To compile the file system:

make

-------------------------

## 🚀 Running the File System

Mount the file system to `/tmp/myfs` using:

chmod +x run_fs.sh
./run_fs.sh

You can now interact with it like a regular directory:

touch /tmp/myfs/hello.txt
echo "Hello, MyFS!" > /tmp/myfs/hello.txt
cat /tmp/myfs/hello.txt

## 🛑 Unmounting

To unmount and stop the file system:

Press `Ctrl+C` in the terminal where the file system is running.

-------------------------

## ⚠️ Limitations

* ❌ Data is **not** persistent — everything is lost on exit
